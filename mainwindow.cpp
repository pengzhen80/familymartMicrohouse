#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
#include<QStackedWidget>
#include<QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_HOMENEW_INDEX);

    numberkeyboard_take_customer = new MyNumberDialog();
//    numberkeyboard_take_customer->btn_finish->setDisabled(true);
    //connect(ui->lineEdit_take_phonenumber,&MyLineEdit::sig_data,this,&MainWindow::show_numberkeyboard_ui);
    connect(numberkeyboard_take_customer,&MyNumberDialog::sendNumber,ui->lineEdit_take_phonenumber,&MyLineEdit::slot_number);
    connect(numberkeyboard_take_customer,&MyNumberDialog::sendMessage,ui->lineEdit_take_phonenumber,&MyLineEdit::slot_message);
    connect(numberkeyboard_take_customer,&MyNumberDialog::sendMessage,this,[&](QString command){
        if(command == "clean")
        {
            if (ui->stackedWidget->currentIndex() == STACK_WIDGET_PAGE_TAKE_CUSTOMER_INDEX) {
                ui->tablewidget_take_customer->clearContents();
                ui->tablewidget_take_customer->setRowCount(0);
            }
        }
    });
    connect(ui->lineEdit_take_phonenumber,&MyLineEdit::sig_numberSizeIsThree,this,[&](){
        Sig_Mainwindow_Itemmanagement sigToItem;
        sigToItem.type = SIG_MAINWINDOWN_ITEMMANAGEMENT_ASKITEMS_BYNUMBER;
        sigToItem.value_askItemsByNumber.number = ui->lineEdit_take_phonenumber->text();
        emit sig_itemmanager(sigToItem);

        qDebug()<<"search item by number:"<<ui->lineEdit_take_phonenumber->text();
    });
    //connect(numberkeyboard,&MyNumberDialog::sendfinish,this,&MainWindow::slot_dialog_finish);

    numberboard_store_multi = new MyNumberDialog();
    numberboard_store_single = new MyNumberDialog();
//    numberboard_store_multi->btn_finish->setDisabled(true);
//    numberboard_store_single->btn_finish->setDisabled(true);

    connect(numberboard_store_single,&MyNumberDialog::sendNumber,ui->lineEdit_store_single_phonenumber,&MyLineEdit::slot_number);
    connect(numberboard_store_single,&MyNumberDialog::sendMessage,ui->lineEdit_store_single_phonenumber,&MyLineEdit::slot_message);

    label_box_moving_ = new QLabel(this);

    //for page store multi's table view
    table_store_multi_model_ = new QStandardItemModel();
    table_store_multi_model_->setColumnCount(4);
    table_store_multi_model_->setHeaderData(0,Qt::Horizontal,QString(tr("姓名")));
    table_store_multi_model_->setHeaderData(1,Qt::Horizontal,QString(tr("手機")));
    table_store_multi_model_->setHeaderData(2,Qt::Horizontal,QString(tr("條形碼")));
    table_store_multi_model_->setHeaderData(3,Qt::Horizontal,QString(tr("選取")));

    ui->tableView_store_multi->setModel(table_store_multi_model_);

    ui->tableView_store_multi->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //hide the row count
    ui->tableView_store_multi->verticalHeader()->hide();

    connect(ui->tableView_store_multi->verticalScrollBar(),&QScrollBar::sliderPressed,this,[&]{
        qDebug()<<"scroll bar pressed";
        if(ui->tableView_store_multi->hasFocus())
        {
            qDebug()<<"clear focus";
//            ui->tableView_store_multi->verticalScrollBar()->clearFocus();
            ui->tableView_store_multi->clearFocus();
            this->setFocus();
        }
    });




    //for take_customer page tablewidget
    ui->tablewidget_take_customer->setColumnCount(5);
    QString tablewidget_take_customer_head = QString("姓名,手機,日期,儲位,取貨");
    QStringList tablewidget_take_customer_headlist = tablewidget_take_customer_head.split(",");
    //QTableWidgetItem *cubesHeaderItem = new QTableWidgetItem(tr("Cubes"));
    //cubesHeaderItem->setIcon(QIcon(QPixmap(":/Images/cubed.png")));
    //cubesHeaderItem->setTextAlignment(Qt::AlignVCenter);
    ui->tablewidget_take_customer->setHorizontalHeaderLabels(tablewidget_take_customer_headlist);
    ui->tablewidget_take_customer->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //hide the row count
    ui->tablewidget_take_customer->verticalHeader()->hide();

    //for take_customer_takepage page tablewidget
    ui->tableWidget_take_takepage->setColumnCount(5);
    QString tableWidget_take_takepage_head = QString("姓名,手機,日期,條形碼,選取");
    QStringList tableWidget_take_takepage_headlist = tableWidget_take_takepage_head.split(",");
    ui->tableWidget_take_takepage->setHorizontalHeaderLabels(tableWidget_take_takepage_headlist);
    ui->tableWidget_take_takepage->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //hide the row count
    ui->tableWidget_take_takepage->verticalHeader()->hide();

    //for take_sevenday's tableview
    table_take_servenday_model_ = new QStandardItemModel();
    table_take_servenday_model_->setColumnCount(5);
    table_take_servenday_model_->setHeaderData(0,Qt::Horizontal,QString(tr("姓名")));
    table_take_servenday_model_->setHeaderData(1,Qt::Horizontal,QString(tr("手機")));
    table_take_servenday_model_->setHeaderData(2,Qt::Horizontal,QString(tr("D+7")));
    table_take_servenday_model_->setHeaderData(3,Qt::Horizontal,QString(tr("日期")));
//    table_take_servenday_model_->setHeaderData(4,Qt::Horizontal,QString(tr("星期")));
    table_take_servenday_model_->setHeaderData(4,Qt::Horizontal,QString(tr("退貨")));

    ui->table_servenday_list->setModel(table_take_servenday_model_);

    ui->table_servenday_list->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //hide the row count
    ui->table_servenday_list->verticalHeader()->hide();

    //for background page tablewidget
    ui->tableWidget_background_itemList->setColumnCount(4);
    QString tableWidget_background_itemList_head = QString("姓名,手機,日期,箱號");
    QStringList tableWidget_background_itemList_headlist = tableWidget_background_itemList_head.split(",");
    ui->tableWidget_background_itemList->setHorizontalHeaderLabels(tableWidget_background_itemList_headlist);
    ui->tableWidget_background_itemList->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //hide the row count
    ui->tableWidget_background_itemList->verticalHeader()->hide();

    //for background show item by boxId page
    ui->tableWidget_background_showItemsByBoxId_showItems->setColumnCount(4);
    QString tableWidget_background_showItemsByBoxId_showItems_head = QString("姓名,手機,日期,箱號");
    QStringList tableWidget_background_showItemsByBoxId_showItems_headlist = tableWidget_background_showItemsByBoxId_showItems_head.split(",");
    ui->tableWidget_background_showItemsByBoxId_showItems->setHorizontalHeaderLabels(tableWidget_background_showItemsByBoxId_showItems_headlist);
    ui->tableWidget_background_showItemsByBoxId_showItems->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //hide the row count
    ui->tableWidget_background_showItemsByBoxId_showItems->verticalHeader()->hide();

    //add take buttons with tips
//    QTextDocument Text_finish;
//    Text_finish.setHtml("<font size='28' color='white'>取貨完成</font><br><font size='4' color='white'>請先関好蓋並拉好門</font>");
//    QPixmap pixmap_finish(Text_finish.size().width(), Text_finish.size().height());
//    pixmap_finish.fill( Qt::transparent );
//    QPainter painter_finish( &pixmap_finish );
//    Text_finish.drawContents(&painter_finish, pixmap_finish.rect());

//    QIcon ButtonIcon_finish(pixmap_finish);
//    ui->btn_take_takepage_finish->setIcon(ButtonIcon_finish);
//    ui->btn_take_takepage_finish->setIconSize(pixmap_finish.rect().size());

//    QTextDocument Text_cancel;
//    Text_cancel.setHtml("<font size='28' color='white'>取消</font><br><font size='4' color='white'>請先関好蓋並拉好門</font>");
//    QPixmap pixmap_cancel(Text_cancel.size().width(), Text_cancel.size().height());
//    pixmap_cancel.fill( Qt::transparent );
//    QPainter painter_cancel( &pixmap_cancel );
//    Text_cancel.drawContents(&painter_cancel, pixmap_cancel.rect());

//    QIcon ButtonIcon_cancel(pixmap_cancel);
//    ui->btn_take_takepage_cancel->setIcon(ButtonIcon_cancel);
//    ui->btn_take_takepage_cancel->setIconSize(pixmap_cancel.rect().size());

    // update label box moving to image
    label_box_moving_img_moving_ = new QImage;
    label_box_moving_img_moving_->load("./pics/tip_boxMoving.png");
    label_box_moving_img_arrive_ = new QImage;
    label_box_moving_img_arrive_->load("./pics/tip_boxArrive.png");

    //update table's select pics
    img_tables_selected_ = new QImage;
    img_tables_selected_->load("./pics/table_selected.png");
    img_tables_unselected_ = new QImage;
    img_tables_unselected_->load("./pics/table_unselect.png");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_HOMENEW_INDEX);
}

void MainWindow::on_btn_home_store_clicked()
{
    ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_STORE_INDEX);
}

void MainWindow::on_btn_home_take_clicked()
{
    ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_TAKEHOME_INDEX);
}

void MainWindow::on_btn_store_back_clicked()
{
    ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_HOMENEW_INDEX);
}

void MainWindow::on_btn_take_back_clicked()
{
    ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_HOMENEW_INDEX);
}

void MainWindow::on_btn_store_takeimage_clicked()
{
    ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_ITEMINFO_INDEX);
}

void MainWindow::on_btn_itemInfo_back_clicked()
{
    ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_HOMENEW_INDEX);
}

void MainWindow::on_btn_store_store_clicked()
{

}

void MainWindow::on_btn_store_finish_clicked()
{
    ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_HOMENEW_INDEX);
}

void MainWindow::on_btn_store_singlestore_clicked()
{   
//    emit sig_dwsmanager_start();

//    ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_STORE_SINGLE_INDEX);
//    ui->lineEdit_store_single_phonenumber->clear();
//    //ui->label_store_single_showImage->setText(tr("這裏顯示DWS圖片"));
//    std::lock_guard<std::mutex> lock(DWS_Management::mutex_box_buffer_);
//    float height = DWS_Management::box_buffer_.height*100;
//    float length = DWS_Management::box_buffer_.length*100;
//    float width = DWS_Management::box_buffer_.width*100;

//    //DWS_Management::Reset_BoxBuff();

//    ui->label_store_single_showImage->setText(QString(tr("高：")+"%1cm"+"\n"+tr("長：")+"%2cm"+"\n"+tr("寬：")+"%3cm")
//                                              .arg(height).arg(length).arg(width));
//    if(height == 0 || length == 0 || width ==0){
//        ui->label_store_single_size->setText(tr("提示：拍攝失敗"));
//        ui->btn_store_single_size_comfirm->setDisabled(true);
//    }
//    else
//    {
//        ui->label_store_single_size->setText(tr("提示：拍攝成功"));
//        ui->btn_store_single_size_comfirm->setEnabled(true);
//    }

//    QPoint pos = ui->lineEdit_store_single_phonenumber->pos();
//    numberboard_store_single->resize(300,280);
//    numberboard_store_single->move(pos.x()+200,pos.y()+150);
//    numberboard_store_single->show();

//    ui->btn_store_single_info_comfirm->setDisabled(true);
//    ui->btn_store_single_size_comfirm->show();
}

void MainWindow::on_btn_store_multistore_clicked()
{
    ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_STORE_MULTI_INDEX);
    QPoint  pos = ui->frame_store_multi_waitingbox->pos();
    label_box_moving_->move(pos.x(),pos.y()+20);
    label_box_moving_->setPixmap(QPixmap::fromImage(*label_box_moving_img_moving_));
    label_box_moving_->resize(label_box_moving_img_moving_->width(),label_box_moving_img_moving_->height());
//    label_box_moving_->setText(QString(tr("物流箱移動中...")));
//    label_box_moving_->resize(ui->btn_store_single_size_comfirm->width(),ui->btn_store_single_size_comfirm->height());
//    label_box_moving_->setAlignment(Qt::AlignCenter);
//    QFont font;
//    font.setPointSize(28);
//    label_box_moving_->setFont(font);
    label_box_moving_->show();

    ui->btn_store_multi_store->setDisabled(true);

    Sig_Mainwindow_Jobmanagement sigToJob;
    sigToJob.type = SIG_MAINWINDOWN_JOBMANAGEMENT_ASK_BOXDOWN_MOSTEMPTY;

    emit sig_jobmanager(sigToJob);
}

void MainWindow::on_btn_store_single_takePicAgain_clicked()
{
//    std::lock_guard<std::mutex> lock(DWS_Management::mutex_box_buffer_);
//    float height = DWS_Management::box_buffer_.height*100;
//    float length = DWS_Management::box_buffer_.length*100;
//    float width = DWS_Management::box_buffer_.width*100;

//    //DWS_Management::Reset_BoxBuff();

//    ui->label_store_single_showImage->setText(QString(tr("高：")+"%1cm"+"\n"+tr("長：")+"%2cm"+"\n"+tr("寬：")+"%3cm")
//                                              .arg(height).arg(length).arg(width));
//    if(height == 0 || length == 0 || width ==0){
//        ui->label_store_single_size->setText(tr("提示：拍攝失敗"));
//    }
//    else
//    {
//        ui->label_store_single_size->setText(tr("提示：拍攝成功"));
//        //ui->btn_store_single_finish->setEnabled(true);
//    }


}

void MainWindow::on_btn_store_single_finish_clicked()
{
    ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_ITEMINFO_INDEX);
}

void MainWindow::on_btn_takehome_customer_clicked()
{
    ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_TAKE_CUSTOMER_INDEX);
    ui->lineEdit_take_phonenumber->clear();
    items_takepage_.clear();
    table_take_customer_map_row_item_.clear();
    //ui->lineEdit_take_phonenumber->setText(QString(tr("請輸入手機末三碼")));

//    QPoint pos = ui->lineEdit_take_phonenumber->pos();
    QPoint pos_homeback = ui->btn_take_customer_back->pos();
//    qDebug()<<"btn back : "<<"x:"<<pos_homeback.x()<<",y:"<<pos_homeback.y();

//    QPoint pos_label_number = ui->label_take_phone->pos();
//    qDebug()<<"label number : "<<"x:"<<pos_label_number.x()<<",y:"<<pos_label_number.y();

//    QPoint pos = ui->frame_take_customer_numberboard->pos();
//    qDebug()<<"frame : "<<"x:"<<pos.x()<<",y:"<<pos.y();
//    numberkeyboard_take_customer->move(pos.x()+495,pos.y()+250);
    numberkeyboard_take_customer->move(pos_homeback.x()+50,pos_homeback.y()+500);
//    QSize lineSize = ui->lineEdit_take_phonenumber->size();
//    numberkeyboard_take_customer->move(pos.x()+lineSize.width()/5,pos.y()+lineSize.height()+50);
    numberkeyboard_take_customer->show();

    this->setFocus();
}

void MainWindow::on_btn_takehome_sevenday_clicked()
{
    count_table_take_servenday_model_selected_ = 0;
    items_takepage_.clear();
    table_take_servenday_map_row_item_.clear();

    Sig_Mainwindow_Itemmanagement sigToItem;
    sigToItem.type = SIG_MAINWINDOWN_ITEMMANAGEMENT_ASKITEMS_BYBEYONDDAY;
    sigToItem.value_askItemsByBeyondDay.day = 7;

    emit sig_itemmanager(sigToItem);

    ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_SEVENDAY_INDEX);
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_HOMENEW_INDEX);
}

void MainWindow::show_numberkeyboard_ui(QString data)
{
//    QPoint pos_numberboard = ui->frame_take_customer_numberboard->pos();
//    numberkeyboard->move(pos_numberboard.x()+200,pos_numberboard.y()+200);
//    numberkeyboard->show();
//    ui->btn_take_customer_back->setDisabled(true);
//    ui->btn_take_customer_take->setDisabled(true);
}

void MainWindow::slot_dialog_finish()
{
    ui->btn_take_customer_back->setEnabled(true);
    ui->btn_take_customer_take->setEnabled(true);
}

void MainWindow::on_btn_store_multi_back_clicked()
{
    ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_HOMENEW_INDEX);
    numberboard_store_multi->hide();
}

void MainWindow::on_btn_store_single_back_clicked()
{
    ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_HOMENEW_INDEX);
    numberboard_store_single->hide();
    emit sig_dwsmanager_stop();
}

void MainWindow::on_btn_store_single_size_comfirm_clicked()
{
    //todo system call transport_sys to down box
    ui->btn_store_single_size_takePicAgain->setDisabled(true);
    ui->btn_store_single_back->setDisabled(true);
    ui->btn_store_single_size_comfirm->hide();

    QPoint  pos = ui->btn_store_single_size_comfirm->pos();
    label_box_moving_->move(pos.x(),pos.y());
    label_box_moving_->setPixmap(QPixmap::fromImage(*label_box_moving_img_moving_));
    label_box_moving_->resize(label_box_moving_img_moving_->width(),label_box_moving_img_moving_->height());

//    label_box_moving_->setText(QString(tr("物流箱移動中...")));
//    //label_box_moving_->setScaledContents(true);
//    label_box_moving_->resize(ui->btn_store_single_size_comfirm->width(),ui->btn_store_single_size_comfirm->height());
//    label_box_moving_->setAlignment(Qt::AlignCenter);
//    QFont font;
//    font.setPointSize(28);
//    label_box_moving_->setFont(font);
    label_box_moving_->show();

//    QTimer::singleShot(TIME_BOX_MOVING_DOWN*1000,this,[&]()
//    {
//        label_box_moving_->hide();
//        ui->btn_store_single_size_comfirm->show();
//        ui->btn_store_single_size_comfirm->setDisabled(true);
//        ui->btn_store_single_info_comfirm->setEnabled(true);
//    });

    emit sig_jobmanager_askFitBoxDown(store_single_temp_item_.size);

    ui->lineEdit_store_single_name->setText(tr("郭朋振"));
}

void MainWindow::on_btn_store_single_info_comfirm_clicked()
{ 
    emit sig_dwsmanager_stop();
    if(ui->lineEdit_store_single_phonenumber->text().isEmpty())
    {
        ui->lineEdit_store_single_phonenumber->setText(tr("請輸入手機末三碼"));
        return;
    }

    store_single_temp_item_.infos.name = ui->lineEdit_store_single_name->text();
    store_single_temp_item_.infos.phone_number = ui->lineEdit_store_single_phonenumber->text();

    label_box_moving_->hide();
    ui->btn_store_single_size_takePicAgain->setEnabled(true);
    ui->btn_store_single_size_comfirm->setEnabled(true);
    ui->btn_store_single_back->setEnabled(true);
    numberboard_store_single->hide();
    ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_STORE_SINGLE_STOREPAGE_INDEX);
    //ui->btn_store_single_storepage_back->setDisabled(true);

    ui->lineEdit_store_single_storepage_name->setText(store_single_temp_item_.infos.name);
    ui->lineEdit_store_single_storepage_name->setDisabled(true);
    ui->lineEdit_store_single_storepage_phonenumber->setText(store_single_temp_item_.infos.phone_number);
    ui->lineEdit_store_single_storepage_phonenumber->setDisabled(true);

    QPoint  pos = ui->frame_store_single_storepage_boxmoving->pos();
    label_box_moving_->move(pos.x(),pos.y());
    label_box_moving_->setPixmap(QPixmap::fromImage(*label_box_moving_img_moving_));
    label_box_moving_->resize(label_box_moving_img_moving_->width(),label_box_moving_img_moving_->height());

//    label_box_moving_->setText(QString(tr("請存入物品")));
//    //label_box_moving_->setScaledContents(true);
//    label_box_moving_->resize(ui->frame_store_single_storepage_boxmoving->width(),ui->frame_store_single_storepage_boxmoving->height());
//    label_box_moving_->setAlignment(Qt::AlignCenter);
//    QFont font;
//    font.setPointSize(28);
//    label_box_moving_->setFont(font);
    label_box_moving_->show();
}

void MainWindow::on_btn_store_single_storepage_back_clicked()
{
    label_box_moving_->setText(QString(tr("物流箱移動中...")));
    ui->btn_store_single_storepage_store->setDisabled(true);
    ui->btn_store_single_storepage_changebox->setDisabled(true);
    ui->btn_store_single_storepage_back->setDisabled(true);
//    QTimer::singleShot(TIME_BOX_MOVING_UP*1000,this,[&]()
//    {
//        label_box_moving_->hide();
//        ui->btn_store_single_storepage_store->setEnabled(true);
//        ui->btn_store_single_storepage_changebox->setEnabled(true);
//        ui->btn_store_single_storepage_back->setEnabled(true);
//        ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_HOMENEW_INDEX);
//    });

    emit sig_jobmanager_JustUpBox();
}

void MainWindow::on_btn_store_multi_store_clicked()
{
    qDebug()<<"MainWindow::on_btn_store_multi_store_clicked";
    //get item data from tableview
    vec_tmp_item_.clear();
    //qDebug()<<table_store_multi_model_->rowCount();
    for(int i_row = 0;i_row < table_store_multi_model_->rowCount();i_row++)
    {
        QString flag = table_store_multi_model_->item(i_row,3)->text();
//        qDebug()<<"MainWindow::on_btn_store_multi_store_clicked : "<<flag;
        if(tr("✓") == flag)
        {
            //QString name = table_store_multi_model_->item(i_row,0)->text();
//            QImage name_image = table_store_multi_model_->item(i_row,0)->data(Qt::DecorationRole).value<QPixmap>().toImage();
            //qDebug()<<table_store_multi_model_->item(i_row,0)->data().toInt();
            //qDebug()<<table_store_multi_model_->item(i_row,0)->data(Qt::DecorationRole);

//            if (table_store_multi_model_->item(i_row,0)->data().canConvert<QImage>())
//            {
//                name_image = table_store_multi_model_->item(i_row,0)->data().value<QImage>();
//                qDebug()<<"MainWindow::on_btn_store_multi_store_clicked name image size is:"<< name_image.width()<<","<<name_image.height();
//            }
//            qDebug()<<"MainWindow::on_btn_store_multi_store_clicked name image size is:"<< name_image.width()<<","<<name_image.height();

            QString name = table_store_multi_model_->item(i_row,0)->text();
            QString number = table_store_multi_model_->item(i_row,1)->text();
            QString barcode = table_store_multi_model_->item(i_row,2)->text();



            Item_FamilyMart item;
            item.infos.name = name;
            item.infos.phone_number = number;
            item.infos.barcode = barcode;
//            item.infos.name_image = name_image;

            vec_tmp_item_.push_back(item);
        }
    }
//    emit sig_jobmanager_ItemInBox(vec_tmp_item_);
    if(vec_tmp_item_.size()>0)
    {
//        Sig_Mainwindow_Itemmanagement sigToItem;
//        sigToItem.type = SIG_MAINWINDOWN_ITEMMANAGEMENT_ADDITEMS;
//        sigToItem.value_addItems.items = vec_tmp_item_;
//        emit sig_itemmanager(sigToItem);
        Sig_Mainwindow_Jobmanagement sigToJob_AddNewItems;
        sigToJob_AddNewItems.type = SIG_MAINWINDOWN_JOBMANAGEMENT_ADDNEWITEMS;
        sigToJob_AddNewItems.value_addNewItems.items = vec_tmp_item_;
        emit sig_jobmanager(sigToJob_AddNewItems);
    }
    else
    {
        Sig_Mainwindow_Jobmanagement sigToJob;
        sigToJob.type = SIG_MAINWINDOWN_JOBMANAGEMENT_ASK_BOXUP;
        emit sig_jobmanager(sigToJob);
    }


    table_store_multi_model_->setRowCount(0);
    count_item_selected_ = 0;


    label_box_moving_->show();
//    ui->btn_store_multi_InfoComfirm->setDisabled(true);
    ui->btn_store_multi_store->setDisabled(true);
    ui->btn_store_multi_changeBox->setDisabled(true);
//    QTimer::singleShot(TIME_BOX_MOVING_UP*1000,this,[&]()
//    {
//        numberboard_store_multi->hide();
//        label_box_moving_->hide();
//        ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_HOMENEW_INDEX);
//    });
    flag_multi_store_changeBox = false;
}

void MainWindow::on_btn_take_customer_take_clicked()
{
//    vec_tmp_item_.clear();
    int tablewidget_take_customer_rowcount = ui->tablewidget_take_customer->rowCount();
    QMap<int,Item_FamilyMart> tmp_table_take_customer_map_row_item = table_take_customer_map_row_item_;
    for(int i=0;i<tablewidget_take_customer_rowcount;i++)
    {
        QString flag = ui->tablewidget_take_customer->item(i,4)->text();
        if(tr("口") == flag)
        {
            table_take_customer_map_row_item_.remove(i);
//            qDebug()<<"remove : "<<i;
        }
    }
    if(0 == table_take_customer_map_row_item_.size())
    {
        table_take_customer_map_row_item_ = tmp_table_take_customer_map_row_item;
        return;
    }
    //next box : set prepare false
    flag_takepage_nextBoxPrepared_  = false;

    numberkeyboard_take_customer->hide();
    ui->tablewidget_take_customer->clearContents();
    ui->tablewidget_take_customer->setRowCount(0);

    ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_TAKE_CUSTOMER_TAKEPAGE_INDEX);

//    for(int i=0;i<vec_tmp_item_.size();i++)
//    {
//        int row = ui->tableWidget_take_customer_takepage->rowCount();
//        ui->tableWidget_take_customer_takepage->setRowCount(++row);

//        //int column = ui->tablewidget_take_customer->columnCount();
//        QTableWidgetItem *name = new QTableWidgetItem(tr("%1").arg(vec_tmp_item_[i].infos.name));
//        name->setTextAlignment(Qt::AlignCenter);
//        QTableWidgetItem *phoneNumber = new QTableWidgetItem(tr("%1").arg(vec_tmp_item_[i].infos.phone_number));
//        phoneNumber->setTextAlignment(Qt::AlignCenter);
//        QTableWidgetItem *dataArriving = new QTableWidgetItem(tr("%1").arg(vec_tmp_item_[i].infos.date_arrive));
//        dataArriving->setTextAlignment(Qt::AlignCenter);

//        ui->tableWidget_take_customer_takepage->setItem(row-1, 0, name);
//        ui->tableWidget_take_customer_takepage->setItem(row-1, 1, phoneNumber);
//        ui->tableWidget_take_customer_takepage->setItem(row-1, 2, dataArriving);
//        ui->tableWidget_take_customer_takepage->resizeRowToContents(row-1);
//        //qDebug()<<"row is "<<ui->tableWidget_take_customer_takepage->rowCount();
//    }
    //todo: show items in table

    QPoint  pos = ui->frame_take_takepage_waitingbox->pos();
    label_box_moving_->move(pos.x(),pos.y()+15);
    label_box_moving_->setPixmap(QPixmap::fromImage(*label_box_moving_img_moving_));
    label_box_moving_->resize(label_box_moving_img_moving_->width(),label_box_moving_img_moving_->height());

//    label_box_moving_->setText(QString(tr("物流箱移動中...")));
//    //label_box_moving_->setScaledContents(true);
//    label_box_moving_->resize(ui->frame_take_takepage_waitingbox->width(),
//                              ui->frame_take_takepage_waitingbox->height());
//    label_box_moving_->setAlignment(Qt::AlignCenter);
//    QFont font;
//    font.setPointSize(28);
//    label_box_moving_->setFont(font);
    label_box_moving_->show();
    ui->btn_take_takepage_finish->setDisabled(true);
    ui->btn_take_takepage_cancel->setDisabled(true);

//    QTimer::singleShot(TIME_BOX_MOVING_DOWN*1000,this,[&]()
//    {
//        label_box_moving_->setText(QString(tr("請取貨")));
//        //label_box_moving_->hide();
//        ui->btn_take_takepage_finish->setEnabled(true);
//        ui->btn_take_takepage_cancel->setEnabled(true);
//    });

    //ask a box down by item
    QList<Item_FamilyMart> itemToTake = table_take_customer_map_row_item_.values();
    QVector<Item_FamilyMart> vec_itemToTake;
    for(int i=0;i<itemToTake.size();i++)
    {
        vec_itemToTake.push_back(itemToTake[i]);
//        qDebug()<<"MainWindow::on_btn_take_customer_take_clicked :"<<itemToTake[i].box.box_id;
    }

    Sig_Mainwindow_Jobmanagement sigToJob;
    sigToJob.type = SIG_MAINWINDOWN_JOBMANAGEMENT_ASK_BOXDOWN_BYITEMS;
    sigToJob.value_askBoxDownByItems.items = vec_itemToTake;

    emit sig_jobmanager(sigToJob);

    //set pre page index for takepage
    pageIndex_prepage_ = STACK_WIDGET_PAGE_TAKE_CUSTOMER_INDEX;
}

void MainWindow::on_btn_take_takepage_finish_clicked()
{
    QVector<int> tmp_selectedRowIndex;
    for(int i_row = 0;i_row < table_store_multi_model_->rowCount();i_row++)
    {
        QString flag = table_store_multi_model_->item(i_row,4)->text();
        if(tr("✓") == flag)
        {
            tmp_selectedRowIndex.push_back(i_row);
        }
    }
    //clear table
    ui->tableWidget_take_takepage->clearContents();
    ui->tableWidget_take_takepage->setRowCount(0);
    take_takepage_hasnextbox_ = false;
    //tell jobmanager to remove items
    qDebug()<<"MainWindow::on_btn_take_customer_takepage_finish_clicked : "<<table_take_customer_tapepagem_itemId_.size();

    Sig_Mainwindow_Itemmanagement sigToItem;
    sigToItem.type = SIG_MAINWINDOWN_ITEMMANAGEMENT_REMOVEITEMS;

    //remove items by selected indexs
    QVector<Item_FamilyMart> tmp_ItemsToRemove;
    for(int i=0;i<tmp_selectedRowIndex.size();i++)
    {
        tmp_ItemsToRemove.push_back(items_takepage_.value(i));
    }

    sigToItem.value_removeItems.items = items_takepage_;
    //sigToItem.value_removeItems.items = tmp_ItemsToRemove;
    emit sig_itemmanager(sigToItem);

    label_box_moving_->setPixmap(QPixmap::fromImage(*label_box_moving_img_moving_));
    label_box_moving_->resize(label_box_moving_img_moving_->width(),label_box_moving_img_moving_->height());

    label_box_moving_->show();

    ui->btn_take_takepage_finish->setDisabled(true);
    ui->btn_take_takepage_cancel->setDisabled(true);

    //todo : wait to flag_takepage_nextBoxPrepared_ = true
    QTimer* timer_check_nextBox = new QTimer();
    connect(timer_check_nextBox,&QTimer::timeout,this,[&,timer_check_nextBox]()
    {
        if(flag_takepage_nextBoxPrepared_)
        {
            Sig_Mainwindow_Jobmanagement sigToJob;
            sigToJob.type = SIG_MAINWINDOWN_JOBMANAGEMENT_ASK_BOXUP;
            emit sig_jobmanager(sigToJob);

            //init data
            flag_takepage_nextBoxPrepared_ = false;
            timer_check_nextBox->stop();
        }
    });
    timer_check_nextBox->start(500);
}

void MainWindow::on_btn_take_takepage_cancel_clicked()
{
    //clear table
    ui->tableWidget_take_takepage->clearContents();
    ui->tableWidget_take_takepage->setRowCount(0);

//    emit sig_jobmanager_JustUpBox();


//    label_box_moving_->setText(QString(tr("物流箱移動中...")));
    label_box_moving_->setPixmap(QPixmap::fromImage(*label_box_moving_img_moving_));
    label_box_moving_->resize(label_box_moving_img_moving_->width(),label_box_moving_img_moving_->height());

    label_box_moving_->show();
    ui->btn_take_takepage_finish->setDisabled(true);
    ui->btn_take_takepage_cancel->setDisabled(true);
//    QTimer::singleShot(TIME_BOX_MOVING_UP*1000,this,[&]()
//    {
//        label_box_moving_->hide();
//        ui->btn_take_takepage_finish->setEnabled(true);
//        ui->btn_take_takepage_cancel->setEnabled(true);
//        ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_HOMENEW_INDEX);
//    });
    //todo : wait to flag_takepage_nextBoxPrepared = true
    //todo : wait to flag_takepage_nextBoxPrepared = true
    QTimer* timer_check_nextBox = new QTimer();
    connect(timer_check_nextBox,&QTimer::timeout,this,[&,timer_check_nextBox]()
    {
        if(flag_takepage_nextBoxPrepared_)
        {
            Sig_Mainwindow_Jobmanagement sigToJob;
            sigToJob.type = SIG_MAINWINDOWN_JOBMANAGEMENT_ASK_BOXUP;
            emit sig_jobmanager(sigToJob);

            //init data
            flag_takepage_nextBoxPrepared_ = false;
            timer_check_nextBox->stop();
        }
    });
    timer_check_nextBox->start(500);

}

void MainWindow::on_btn_take_sevenday_take_clicked()
{
     int tableview_take_sevenday_rowcount = table_take_servenday_model_->rowCount();

     QMap<int,Item_FamilyMart> tmp_servenday_map_row_item = table_take_servenday_map_row_item_;

     for(int i=0;i<tableview_take_sevenday_rowcount;i++)
     {
         QString flag = table_take_servenday_model_->item(i,4)->text();
//         qDebug()<<"MainWindow::on_btn_take_sevenday_take_clicked : "<<flag;
         if(tr("口") == flag)
         {
             tmp_servenday_map_row_item.remove(i);
         }
     }

     if(tmp_servenday_map_row_item.size()  == 0)
     {
         return;
     }
     //next box : set prepare false
     flag_takepage_nextBoxPrepared_  = false;

    //ask a box down by item
    QList<Item_FamilyMart> itemToTake = tmp_servenday_map_row_item.values();
    QVector<Item_FamilyMart> vec_itemToTake;
    for(int i=0;i<itemToTake.size();i++)
    {
        vec_itemToTake.push_back(itemToTake[i]);
    }
//    emit sig_jobmanager_askBoxDownByItems(vec_itemToTake);
    Sig_Mainwindow_Jobmanagement sigToJob;
    sigToJob.type = SIG_MAINWINDOWN_JOBMANAGEMENT_ASK_BOXDOWN_BYITEMS;
    sigToJob.value_askBoxDownByItems.items = vec_itemToTake;
    emit sig_jobmanager(sigToJob);

    ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_TAKE_CUSTOMER_TAKEPAGE_INDEX);
    //todo: show items in table
    QPoint  pos = ui->frame_take_takepage_waitingbox->pos();
    label_box_moving_->move(pos.x(),pos.y()+15);
    label_box_moving_->setPixmap(QPixmap::fromImage(*label_box_moving_img_moving_));
    label_box_moving_->resize(label_box_moving_img_moving_->width(),label_box_moving_img_moving_->height());

//    label_box_moving_->setText(QString(tr("物流箱移動中...")));
//    //label_box_moving_->setScaledContents(true);
//    label_box_moving_->resize(ui->frame_take_takepage_waitingbox->width(),
//                              ui->frame_take_takepage_waitingbox->height());
//    label_box_moving_->setAlignment(Qt::AlignCenter);
//    QFont font;
//    font.setPointSize(28);
//    label_box_moving_->setFont(font);
    label_box_moving_->show();
    ui->btn_take_takepage_finish->setDisabled(true);
    ui->btn_take_takepage_cancel->setDisabled(true);

    //set pre page for takepage
    pageIndex_prepage_ = STACK_WIDGET_PAGE_SEVENDAY_INDEX;
}

void MainWindow::on_btn_take_sevenday_back_clicked()
{
    ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_HOMENEW_INDEX);
}

void MainWindow::on_btn_store_single_storepage_store_clicked()
{
    //todo: save item to DB
//    label_box_moving_->setText(QString(tr("物流箱移動中...")));
    label_box_moving_->setPixmap(QPixmap::fromImage(*label_box_moving_img_moving_));
    label_box_moving_->resize(label_box_moving_img_moving_->width(),label_box_moving_img_moving_->height());

    ui->btn_store_single_storepage_store->setDisabled(true);
    ui->btn_store_single_storepage_changebox->setDisabled(true);
    ui->btn_store_single_storepage_back->setDisabled(true);
//    QTimer::singleShot(TIME_BOX_MOVING_UP*1000,this,[&]()
//    {
//        label_box_moving_->hide();
//        ui->btn_store_single_storepage_store->setEnabled(true);
//        ui->btn_store_single_storepage_changebox->setEnabled(true);
//        ui->btn_store_single_storepage_back->setEnabled(true);
//        ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_HOMENEW_INDEX);
//    });

    QVector<Item_FamilyMart> vec_item;
    vec_item.push_back(store_single_temp_item_);

    emit sig_jobmanager_ItemInBox(vec_item);
}

void MainWindow::on_btn_store_single_storepage_changebox_clicked()
{
//    label_box_moving_->setText(QString(tr("物流箱移動中...")));
    label_box_moving_->setPixmap(QPixmap::fromImage(*label_box_moving_img_moving_));
    label_box_moving_->resize(label_box_moving_img_moving_->width(),label_box_moving_img_moving_->height());

    ui->btn_store_single_storepage_store->setDisabled(true);
    ui->btn_store_single_storepage_changebox->setDisabled(true);
    ui->btn_store_single_storepage_back->setDisabled(true);
    QTimer::singleShot(TIME_BOX_MOVING_UP*1000,this,[&]()
    {
        //todo tell transport-sys to up box
        //tell transport management to down another box by size
        QTimer::singleShot(TIME_BOX_MOVING_DOWN*1000,this,[&]()
        {
            label_box_moving_->setText(QString(tr("請存入物品")));
            ui->btn_store_single_storepage_store->setEnabled(true);
            ui->btn_store_single_storepage_changebox->setEnabled(true);
            ui->btn_store_single_storepage_back->setEnabled(true);
        });
    });

    emit sig_jobmanager_ChangeBoxMoreFit(store_single_temp_item_);
}

void MainWindow::on_btn_take_customer_back_clicked()
{
    numberkeyboard_take_customer->hide();
    ui->tablewidget_take_customer->clearContents();
    ui->tablewidget_take_customer->setRowCount(0);
    ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_HOMENEW_INDEX);
    numberkeyboard_take_customer->hide();
}

void MainWindow::on_btn_store_single_size_takePicAgain_clicked()
{
//    std::lock_guard<std::mutex> lock(DWS_Management::mutex_box_buffer_);
//    float height = DWS_Management::box_buffer_.height*100;
//    float length = DWS_Management::box_buffer_.length*100;
//    float width = DWS_Management::box_buffer_.width*100;

//    //DWS_Management::Reset_BoxBuff();

//    ui->label_store_single_showImage->setText(QString(tr("高：")+"%1cm"+"\n"+tr("長：")+"%2cm"+"\n"+tr("寬：")+"%3cm")
//                                              .arg(height).arg(length).arg(width));
//    if(height == 0 || length == 0 || width ==0){
//        ui->label_store_single_size->setText(tr("提示：拍攝失敗"));
//        //ui->->setDisabled(true);
//    }
//    else
//    {
//        ui->label_store_single_size->setText(tr("提示：拍攝成功"));
//        store_single_temp_item_.size.height = height;
//        store_single_temp_item_.size.length = length;
//        store_single_temp_item_.size.width = width;
//        ui->btn_store_single_size_comfirm->setEnabled(true);
//    }
}

void MainWindow::on_btn_store_multi_InfoComfirm_clicked()
{
////    QString name = ui->lineEdit_store_multi_name->text();
//    QString number = ui->lineEdit_store_multi_phonenumber->text();
//    ui->lineEdit_store_multi_phonenumber->clear();
//    if(ui->label_store_multi_name->pixmap() == nullptr)
//    {
//        qDebug()<<"ui->label_store_multi_name has no image";
//        return;
//    }
//    QImage name_img = ui->label_store_multi_name->pixmap()->toImage();
//    qDebug()<<"name image is :"<<name_img.width()<<" "<<name_img.height();
//    ui->label_store_multi_name->clear();

//    if(number.size()==3)
//    {
//        Item_FamilyMart item;
////        item.infos.name = name;
//        item.infos.phone_number = number;
//        item.infos.name_image = name_img;
//        //name_img.scaled(400,100);
//        vec_tmp_item_.push_back(item);

//        //QStandardItem(QIcon(QPixmap::fromImage(nameImage_)),QString(""));
//        //table_store_multi_model_->setItem(vec_tmp_item_.size()-1,0,new QStandardItem(name));
//        QStandardItem* item_name_img = new QStandardItem();
//        item_name_img->setData(QVariant(QPixmap::fromImage(name_img)), Qt::DecorationRole);
//        table_store_multi_model_->setItem(vec_tmp_item_.size()-1,0,item_name_img);
//        table_store_multi_model_->setItem(vec_tmp_item_.size()-1,1,new QStandardItem(number));
//        table_store_multi_model_->setItem(vec_tmp_item_.size()-1,2,new QStandardItem(tr("✓")));
//        //table_store_multi_model_->item(vec_tmp_item_.size()-1,0)->setTextAlignment(Qt::AlignCenter);
//        table_store_multi_model_->item(vec_tmp_item_.size()-1,1)->setTextAlignment(Qt::AlignCenter);
//        table_store_multi_model_->item(vec_tmp_item_.size()-1,2)->setTextAlignment(Qt::AlignCenter);

//        ui->tableView_store_multi->resizeRowToContents(vec_tmp_item_.size()-1);

//        //todo scan again
//        emit sig_clipboardmanager_askNameImage();
//        emit sig_clipboardmanager_askNumberImage();
//    }
}

void MainWindow::on_tableView_store_multi_clicked(const QModelIndex &index)
{
    if(index.isValid())
    {
        if(index.column() == 3)
        {
//            qDebug()<<table_store_multi_model_->itemData(index).value(2).toString();
//            qDebug()<<table_store_multi_model_->itemData(index).value(0).toString();
//            qDebug()<<table_store_multi_model_->itemData(index).value(1).toString();
            if(table_store_multi_model_->itemData(index).value(0).toString() ==tr("口"))
            {
                table_store_multi_model_->setItem(index.row(),3,new QStandardItem(tr("✓")));
                table_store_multi_model_->item(index.row(),3)->setTextAlignment(Qt::AlignCenter);

                count_item_selected_++;

//                ui->btn_store_multi_store->setText(QString(tr("%1 筆入庫").arg(count_item_selected_)));
            }
            else if(table_store_multi_model_->itemData(index).value(0).toString() == tr("✓"))
            {
                table_store_multi_model_->setItem(index.row(),3,new QStandardItem("口"));
                table_store_multi_model_->item(index.row(),3)->setTextAlignment(Qt::AlignCenter);
                count_item_selected_--;
//                ui->btn_store_multi_store->setText(QString(tr("%1 筆入庫").arg(count_item_selected_)));
            }
        }

    }
    if(ui->tableView_store_multi->hasFocus())
    {
        ui->tableView_store_multi->clearFocus();
        this->setFocus();
    }
}

void MainWindow::show_ItemsInfoSearchedByPhoneNumber(QVector<Item_FamilyMart> vec_item)
{
    qDebug()<<"get item size is "<<vec_item.size();
//    ui->tablewidget_take_customer->clearContents();
    for(int i=0;i<vec_item.size();i++)
    {
        int row = ui->tablewidget_take_customer->rowCount();
        ui->tablewidget_take_customer->setRowCount(++row);

        //int column = ui->tablewidget_take_customer->columnCount();
        QTableWidgetItem *name = new QTableWidgetItem(tr("%1").arg(vec_item[i].infos.name));
        name->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *phoneNumber = new QTableWidgetItem(tr("%1").arg(vec_item[i].infos.phone_number));
        phoneNumber->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem *dataArriving = new QTableWidgetItem(tr("%1").arg(vec_item[i].infos.date_arrive));
        dataArriving->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem *boxId = new QTableWidgetItem(tr("%1").arg(vec_item[i].box.box_id));
        boxId->setTextAlignment(Qt::AlignCenter);

//        qDebug()<<"take customer :"<<vec_item[i].infos.name_image.width()<<vec_item[i].infos.name_image.height();
//        QTableWidgetItem* icon_unselect = new QTableWidgetItem();
//        icon_unselect->setData(Qt::DecorationRole,QVariant(QPixmap::fromImage(*img_tables_unselected_)));


        QTableWidgetItem *flagTake = new QTableWidgetItem(tr("口"));
        flagTake->setTextAlignment(Qt::AlignCenter);

        ui->tablewidget_take_customer->setItem(row-1,0,name);
        ui->tablewidget_take_customer->setItem(row-1, 1, phoneNumber);
        ui->tablewidget_take_customer->setItem(row-1, 2, dataArriving);
        ui->tablewidget_take_customer->setItem(row-1, 3, boxId);
        ui->tablewidget_take_customer->setItem(row-1, 4, flagTake);
        ui->tablewidget_take_customer->resizeRowToContents(row-1);
        //qDebug()<<"row is "<<ui->tablewidget_take_customer->rowCount();

        table_take_customer_map_row_item_.insert(row-1,vec_item[i]);
        //qDebug()<<"MainWindow::slot_jobmanager_ItemsInfoSearchedByPhoneNumber:"<<vec_item[i].box.box_id;
    }
}

void MainWindow::on_tablewidget_take_customer_cellClicked(int row, int column)
{
    if(4 == column)
    {
        if(ui->tablewidget_take_customer->item(row,column)->text() == tr("口"))
        {
            QTableWidgetItem *flagTake = new QTableWidgetItem(tr("✓"));
            flagTake->setTextAlignment(Qt::AlignCenter);

            ui->tablewidget_take_customer->setItem(row, 4, flagTake);
        }
        else if(ui->tablewidget_take_customer->item(row,column)->text() == tr("✓"))
        {
            QTableWidgetItem *flagTake = new QTableWidgetItem(tr("口"));
            flagTake->setTextAlignment(Qt::AlignCenter);

            ui->tablewidget_take_customer->setItem(row, 4, flagTake);
        }
    }
}

void MainWindow::show_ItemsByBoxId(QVector<Item_FamilyMart> vec_item)
{
    table_take_customer_tapepagem_itemId_.clear();

//    qDebug()<<"MainWindow::slot_jobmanager_ItemsInBox : get item size is "<<vec_item.size();
    ui->tableWidget_take_takepage->clearContents();
    ui->tableWidget_take_takepage->setRowCount(0);

    //store item id to set
    QSet<QString> set_ItemId;
    for(int i=0;i<items_takepage_.size();i++)
    {
        if(set_ItemId.contains(items_takepage_[i].id))
        {

        }
        else
        {
            set_ItemId.insert(items_takepage_[i].id);
        }
    }

    for(int i=0;i<vec_item.size();i++)
    {
        if(!set_ItemId.contains(vec_item[i].id))
        {
            continue;
        }

        int row = ui->tableWidget_take_takepage->rowCount();
        ui->tableWidget_take_takepage->setRowCount(++row);

        //int column = ui->tablewidget_take_customer->columnCount();
//        QTableWidgetItem *name = new QTableWidgetItem(tr("%1").arg(vec_item[i].infos.name));
//        name->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem* item_name_img = new QTableWidgetItem();
        item_name_img->setData(Qt::DecorationRole,QVariant(QPixmap::fromImage(vec_item[i].infos.name_image)));

        QTableWidgetItem *phoneNumber = new QTableWidgetItem(tr("%1").arg(vec_item[i].infos.phone_number));
        phoneNumber->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem *dataArriving = new QTableWidgetItem(tr("%1").arg(vec_item[i].infos.date_arrive));
        dataArriving->setTextAlignment(Qt::AlignCenter);

        //ui->tableWidget_take_takepage->setItem(row-1, 0, name);
        ui->tableWidget_take_takepage->setItem(row-1,0,item_name_img);
        ui->tableWidget_take_takepage->setItem(row-1, 1, phoneNumber);
        ui->tableWidget_take_takepage->setItem(row-1, 2, dataArriving);
        ui->tableWidget_take_takepage->resizeRowToContents(row-1);
        //qDebug()<<"row is "<<ui->tablewidget_take_customer->rowCount();

        table_take_customer_tapepagem_itemId_.push_back(vec_item[i].id);
    }
    take_takepage_hasnextbox_ = true;
}

void MainWindow::show_ItemsInfo_takePage(QVector<Item_FamilyMart> vec_item)
{
    table_take_customer_tapepagem_itemId_.clear();

//    qDebug()<<"MainWindow::slot_jobmanager_ItemsInBox : get item size is "<<vec_item.size();
    ui->tableWidget_take_takepage->clearContents();
    ui->tableWidget_take_takepage->setRowCount(0);

    for(int i=0;i<vec_item.size();i++)
    {
        int row = ui->tableWidget_take_takepage->rowCount();
        ui->tableWidget_take_takepage->setRowCount(++row);

        //int column = ui->tablewidget_take_customer->columnCount();
//        QTableWidgetItem *name = new QTableWidgetItem(tr("%1").arg(vec_item[i].infos.name));
//        name->setTextAlignment(Qt::AlignCenter);

//        QTableWidgetItem* item_name_img = new QTableWidgetItem();
//        item_name_img->setData(Qt::DecorationRole,QVariant(QPixmap::fromImage(vec_item[i].infos.name_image)));
        QTableWidgetItem *name = new QTableWidgetItem(tr("%1").arg(vec_item[i].infos.name));
        name->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *phoneNumber = new QTableWidgetItem(tr("%1").arg(vec_item[i].infos.phone_number));
        phoneNumber->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem *dataArriving = new QTableWidgetItem(tr("%1").arg(vec_item[i].infos.date_arrive));
        dataArriving->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem *barcode = new QTableWidgetItem(tr("%1").arg(vec_item[i].infos.barcode));
        barcode->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem *select_flag = new QTableWidgetItem(tr("%1").arg(tr("口")));
        select_flag->setTextAlignment(Qt::AlignCenter);

        //ui->tableWidget_take_takepage->setItem(row-1, 0, name);
        ui->tableWidget_take_takepage->setItem(row-1,0,name);
        ui->tableWidget_take_takepage->setItem(row-1, 1, phoneNumber);
        ui->tableWidget_take_takepage->setItem(row-1, 2, dataArriving);
        ui->tableWidget_take_takepage->setItem(row-1, 3, barcode);
        ui->tableWidget_take_takepage->setItem(row-1, 4, select_flag);
        ui->tableWidget_take_takepage->resizeRowToContents(row-1);
        //qDebug()<<"row is "<<ui->tablewidget_take_customer->rowCount();

        table_take_customer_tapepagem_itemId_.push_back(vec_item[i].id);
    }
    take_takepage_hasnextbox_ = true;
}

void MainWindow::takepage_selectRowByBarcode(QString barcode)
{
    qDebug()<<"select by barcode : "<<barcode;
    if(ui->stackedWidget->currentIndex() == STACK_WIDGET_PAGE_TAKE_CUSTOMER_TAKEPAGE_INDEX)
    {
        for(int i_row = 0;i_row < ui->tableWidget_take_takepage->rowCount();i_row++)
        {
            QString row_barcode = ui->tableWidget_take_takepage->item(i_row,3)->text();

            if(barcode == row_barcode)
            {
                QString flag = ui->tableWidget_take_takepage->item(i_row,3)->text();
                if(tr("口") == flag)
                {
                    ui->tableWidget_take_takepage->item(i_row,3)->setText(tr("✓"));
                    qDebug()<<"select by barcode : success ";
                }
                else
                {
                    qDebug()<<"select by barcode : already selected";
                }
            }
        }
    }
}

void MainWindow::slot_jobmanager_boxUpArrive()
{
    int currentPage = ui->stackedWidget->currentIndex();

    switch (currentPage)
    {
    case STACK_WIDGET_PAGE_STORE_SINGLE_INDEX:
    {
        //todo
        break;
    }
    case STACK_WIDGET_PAGE_STORE_MULTI_INDEX:
    {
        qDebug()<<"store multi box arrive";
        numberboard_store_multi->hide();
        label_box_moving_->hide();
        ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_HOMENEW_INDEX);
        break;
    }
    case STACK_WIDGET_PAGE_TAKE_CUSTOMER_TAKEPAGE_INDEX:
    {
        label_box_moving_->hide();
        ui->btn_take_takepage_finish->setEnabled(true);
        ui->btn_take_takepage_cancel->setEnabled(true);
        ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_HOMENEW_INDEX);
    }
    }
}

void MainWindow::slot_jobmanager_boxDownArrive()
{
    int currentPage = ui->stackedWidget->currentIndex();
    qDebug()<<currentPage;
    switch (currentPage)
    {
    case STACK_WIDGET_PAGE_STORE_SINGLE_INDEX:
    {
        //todo
        break;
    }
    case STACK_WIDGET_PAGE_STORE_MULTI_INDEX:
    {
        label_box_moving_->hide();
//        ui->btn_store_multi_InfoComfirm->setEnabled(true);
        ui->btn_store_multi_store->setEnabled(true);
        break;
    }
    case STACK_WIDGET_PAGE_TAKE_CUSTOMER_TAKEPAGE_INDEX:
    {
//        label_box_moving_->setText(QString(tr("請取貨")));
        label_box_moving_->setPixmap(QPixmap::fromImage(*label_box_moving_img_arrive_));
        label_box_moving_->resize(label_box_moving_img_arrive_->width(),label_box_moving_img_arrive_->height());

        //label_box_moving_->hide();
        ui->btn_take_takepage_finish->setEnabled(true);
        ui->btn_take_takepage_cancel->setEnabled(true);
    }
    }
}

void MainWindow::slot_jobmanager_modbus_errors(QVector<QString> errors)
{
    if(ui->stackedWidget->currentIndex() != recover_page_index_)
    {
        recover_page_index_ = ui->stackedWidget->currentIndex();
        ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_PLC_ERROR_INDEX);

        QString text("");
        for(int i=0;i<errors.size();i++)
        {
            text += errors[i]+"\n";
        }
        ui->textBrowser_plc_errors->setText(text);
    }
    else
    {
        QString text("");
        for(int i=0;i<errors.size();i++)
        {
            text += errors[i]+"\n";
        }
        ui->textBrowser_plc_errors->setText(text);
    }

}
void MainWindow::slot_jobmanager_modbux_fixed()
{
    if(STACK_WIDGET_PAGE_PLC_ERROR_INDEX ==  ui->stackedWidget->currentIndex())
    {
        ui->stackedWidget->setCurrentIndex(recover_page_index_);
    }
}

void MainWindow::slot_jobmanager(Sig_Jobmanagement_Mainwindow signal)
{
    switch (signal.type) {
    case SIG_JOBMANAGEMENT_MAINWINDOW_MODBUS_RECOVERY_SUCCESS:
    {
        ui->btn_background_plc_recovery->setDisabled(true);
        break;
    }
    case SIG_JOBMANAGEMENT_MAINWINDOW_BOXDOWN:
    {
        int boxId = signal.value_boxDown.boxId;
        boxDown(boxId);

        break;
    }
    case SIG_JOBMANAGEMENT_MAINWINDOW_BOX_XYIN:
    {
        boxInFinished();
        break;
    }
    case SIG_JOBMANAGEMENT_MAINWINDOW_NEXT_BOX_PREPARED:
    {
        flag_takepage_nextBoxPrepared_ = true;
        qDebug()<<"next box prepared or no next box";
        break;
    }
    default:
    {
        qDebug()<<"MainWindow::slot_jobmanager : unknown signal";
        break;
    }
    }
}

void MainWindow::show_allItems(QVector<Item_FamilyMart> vec_item)
{
    if(ui->stackedWidget->currentIndex() == STACK_WIDGET_PAGE_BACKGROUD_INDEX)
    {
        //todo sort items

        table_take_customer_tapepagem_itemId_.clear();

        qDebug()<<"MainWindow::slot_jobmanager_allItems : get item size is "<<vec_item.size();
        ui->tableWidget_background_itemList->clearContents();
        ui->tableWidget_background_itemList->setRowCount(0);


        QVector<Item_FamilyMart> sorted_items = sortItemsByNameAndPhone(vec_item);
        for(int i=0;i<sorted_items.size();i++)
        {
            int row = ui->tableWidget_background_itemList->rowCount();
            ui->tableWidget_background_itemList->setRowCount(++row);

//            QTableWidgetItem* item_name_img = new QTableWidgetItem();
//            item_name_img->setData(Qt::DecorationRole,QVariant(QPixmap::fromImage(vec_item[i].infos.name_image)));
            QTableWidgetItem *name = new QTableWidgetItem(tr("%1").arg(sorted_items[i].infos.name));
            name->setTextAlignment(Qt::AlignCenter);
            QTableWidgetItem *phoneNumber = new QTableWidgetItem(tr("%1").arg(sorted_items[i].infos.phone_number));
            phoneNumber->setTextAlignment(Qt::AlignCenter);
            QTableWidgetItem *dataArriving = new QTableWidgetItem(tr("%1").arg(sorted_items[i].infos.date_arrive));
            dataArriving->setTextAlignment(Qt::AlignCenter);
            QTableWidgetItem *boxId = new QTableWidgetItem(tr("%1").arg(sorted_items[i].box.box_id));
            boxId->setTextAlignment(Qt::AlignCenter);

            //ui->tableWidget_take_takepage->setItem(row-1, 0, name);
            ui->tableWidget_background_itemList->setItem(row-1,0,name);
            ui->tableWidget_background_itemList->setItem(row-1, 1, phoneNumber);
            ui->tableWidget_background_itemList->setItem(row-1, 2, dataArriving);
            ui->tableWidget_background_itemList->setItem(row-1, 3, boxId);
            ui->tableWidget_background_itemList->resizeRowToContents(row-1);
            //qDebug()<<"row is "<<ui->tablewidget_take_customer->rowCount();
        }
    }
    else if(ui->stackedWidget->currentIndex() == STACK_WIDGET_PAGE_BACKGROUD_SHOWITEMSBYBOXID_INDEX)
    {
        //todo
        ui->tableWidget_background_showItemsByBoxId_showItems->clearContents();
        ui->tableWidget_background_showItemsByBoxId_showItems->setRowCount(0);

        QVector<Item_FamilyMart> sorted_items = sortItemsByBoxId(vec_item);
        for(int i=0;i<sorted_items.size();i++)
        {
            int row = ui->tableWidget_background_showItemsByBoxId_showItems->rowCount();
            ui->tableWidget_background_showItemsByBoxId_showItems->setRowCount(++row);

//            QTableWidgetItem* item_name_img = new QTableWidgetItem();
//            item_name_img->setData(Qt::DecorationRole,QVariant(QPixmap::fromImage(vec_item[i].infos.name_image)));
            QTableWidgetItem *name = new QTableWidgetItem(tr("%1").arg(sorted_items[i].infos.name));
            name->setTextAlignment(Qt::AlignCenter);
            QTableWidgetItem *phoneNumber = new QTableWidgetItem(tr("%1").arg(sorted_items[i].infos.phone_number));
            phoneNumber->setTextAlignment(Qt::AlignCenter);
            QTableWidgetItem *dataArriving = new QTableWidgetItem(tr("%1").arg(sorted_items[i].infos.date_arrive));
            dataArriving->setTextAlignment(Qt::AlignCenter);
            QTableWidgetItem *boxId = new QTableWidgetItem(tr("%1").arg(sorted_items[i].box.box_id));
            boxId->setTextAlignment(Qt::AlignCenter);

            //ui->tableWidget_take_takepage->setItem(row-1, 0, name);
            ui->tableWidget_background_showItemsByBoxId_showItems->setItem(row-1,0,name);
            ui->tableWidget_background_showItemsByBoxId_showItems->setItem(row-1, 1, phoneNumber);
            ui->tableWidget_background_showItemsByBoxId_showItems->setItem(row-1, 2, dataArriving);
            ui->tableWidget_background_showItemsByBoxId_showItems->setItem(row-1, 3, boxId);
            ui->tableWidget_background_showItemsByBoxId_showItems->resizeRowToContents(row-1);
            //qDebug()<<"row is "<<ui->tablewidget_take_customer->rowCount();
        }
    }
}

void MainWindow::show_ItemByBarcode(Item_FamilyMart item)
{
    if(ui->stackedWidget->currentIndex() == STACK_WIDGET_PAGE_STORE_MULTI_INDEX)
    {
        qDebug()<<"get an item:"<<item.infos.name;
        qDebug()<<item.infos.phone_number;
        QString name = item.infos.name;
        QString phoneNumber = item.infos.phone_number;
        QString barcode = item.infos.barcode;
        if(name.isEmpty()||phoneNumber.isEmpty())
        {
            return;
        }
//        vec_tmp_item_.push_back(item);
//        qDebug()<<"tmp vec is "<<vec_tmp_item_.size();
//        table_store_multi_model_->setItem(vec_tmp_item_.size()-1,0,new QStandardItem(name));
//        table_store_multi_model_->setItem(vec_tmp_item_.size()-1,1,new QStandardItem(phoneNumber));
//        table_store_multi_model_->setItem(vec_tmp_item_.size()-1,2,new QStandardItem(tr("✓")));

//        table_store_multi_model_->item(vec_tmp_item_.size()-1,0)->setTextAlignment(Qt::AlignCenter);
//        table_store_multi_model_->item(vec_tmp_item_.size()-1,1)->setTextAlignment(Qt::AlignCenter);
//        table_store_multi_model_->item(vec_tmp_item_.size()-1,2)->setTextAlignment(Qt::AlignCenter);

//        QList<QStandardItem *> table_items;
//        table_items.append(new QStandardItem(name));
//        table_items.append(new QStandardItem(phoneNumber));
//        table_items.append(new QStandardItem(tr("✓")));
//        table_store_multi_model_->appendRow(table_items);

        QList<QStandardItem *> table_items;
        QStandardItem *Item_name = new QStandardItem(name);
        Item_name->setTextAlignment(Qt::AlignCenter);
        table_items.append(Item_name);
        QStandardItem *Item_phone = new QStandardItem(phoneNumber);
        Item_phone->setTextAlignment(Qt::AlignCenter);
        table_items.append(Item_phone);

        QStandardItem *Item_barcode = new QStandardItem(barcode);
        Item_barcode->setTextAlignment(Qt::AlignCenter);
        table_items.append(Item_barcode);

        QStandardItem *Item_flag = new QStandardItem(tr("✓"));
        Item_flag->setTextAlignment(Qt::AlignCenter);
        table_items.append(Item_flag);
        table_store_multi_model_->appendRow(table_items);

    //    ui->tableView_store_multi->resizeRowToContents(vec_tmp_item_.size()-1);
        ui->tableView_store_multi->resizeRowsToContents();
    }
}

void MainWindow::slot_jobmanager_emptyBoxDetected()
{
//    qDebug()<<"MainWindow::slot_jobmanager_emptyBoxDetected :get a sig";
    ui->btn_background_back->setEnabled(true);
    ui->btn_background_detectEmptyBox->setEnabled(true);
    ui->btn_background_searchItem->setEnabled(true);
}

//void MainWindow::slot_dwsmanager_boxsizeupdate(DWS_Box buff)
//{
//    if(ui->stackedWidget->currentIndex() == STACK_WIDGET_PAGE_STORE_SINGLE_INDEX)
//    {
//        std::lock_guard<std::mutex> lock(DWS_Management::mutex_box_buffer_);
//        float height = buff.height*100;
//        float length = buff.length*100;
//        float width = buff.width*100;

//        //DWS_Management::Reset_BoxBuff();

//        ui->label_store_single_showImage->setText(QString(tr("高：")+"%1cm"+"\n"+tr("長：")+"%2cm"+"\n"+tr("寬：")+"%3cm")
//                                                  .arg(height).arg(length).arg(width));
//        if(height == 0 || length == 0 || width ==0){
//            ui->label_store_single_size->setText(tr("提示：拍攝失敗"));
//            //ui->->setDisabled(true);
//        }
//        else
//        {
//            ui->label_store_single_size->setText(tr("提示：拍攝成功"));
//            store_single_temp_item_.size.height = height;
//            store_single_temp_item_.size.length = length;
//            store_single_temp_item_.size.width = width;
//            ui->btn_store_single_size_comfirm->setEnabled(true);
//        }
//    }
//}
void MainWindow::slot_jobmanager_phoneNumber(QString number)
{
    int currentPage = ui->stackedWidget->currentIndex();
    switch (currentPage) {
    case STACK_WIDGET_PAGE_STORE_MULTI_INDEX:
    {
//        ui->lineEdit_store_multi_phonenumber->setText(number);
        break;
    }
    }
}

void MainWindow::slot_itemmanager(Sig_ItemManagement_Mainwindow sig)
{
    switch (sig.type) {
    case SIG_ITEMMANAGEMENT_MAINWINDOW_ITEMS_BYNUMBER:
    {
        show_ItemsInfoSearchedByPhoneNumber(sig.value_itemsByNumber.items);
        break;
    }
    case SIG_ITEMMANAGEMENT_MAINWINDOW_ITEMS_BYDAY:
    {
        show_ItemsInfoSearchedByDay(sig.value_itemsByDay.items);
        break;
    }
    case SIG_ITEMMANAGEMENT_MAINWINDOW_ITEMS_ALL:
    {
        show_allItems(sig.value_itemsAll.items);
        break;
    }
    case SIG_ITEMMANAGEMENT_MAINWINDOW_ITEM_BYBARCODE:
    {
        show_ItemByBarcode(sig.value_itemByBarcode.item);
        break;
    }
    case SIG_ITEMMANAGEMENT_MAINWINDOW_ITEMS_BYBOXID:
    {
//        items_takepage_ = sig.value_itemsByBoxId.items;
//        show_ItemsByBoxId(sig.value_itemsByBoxId.items);
        //no need
        break;
    }
    default:
    {
        qDebug()<<"MainWindow::slot_itemmanager : unknown signal";
        break;
    }
    }
}

void MainWindow::slot_clipboard(Sig_Clipmanagement_Mainwindow sig)
{
    switch (sig.type) {
    case SIG_CLIPMANAGEMENT_MAINWINDOW_BARCODE:
    {
        if(STACK_WIDGET_PAGE_STORE_MULTI_INDEX == ui->stackedWidget->currentIndex())
        {
            multiStore_getBarcode(sig.value_barcode.barcode);
        }
        break;
    }
    default:
    {
        qDebug()<<"MainWindow::slot_clipboard : unknown signal";
    }
    }
}

void MainWindow::slot_modbusmanager(Sig_ModbusManagement_MainWindow sig)
{
    switch (sig.type) {
    case SIG_MODBUSMANAGEMENT_MAINWINDOW_ELEVATOR_DOWN:
    {
        ui->btn_background_askBoxDownByBtn_store->setEnabled(true);
        break;
    }
    case SIG_MODBUSMANAGEMENT_MAINWINDOW_ELEVATOR_UP:
    {
        Sig_Mainwindown_Modbusmanagement sigToModbus;
        sigToModbus.type = SIG_MAINWINDOWN_MODBUSMANAGEMENT_XY_PUTIN;
        sigToModbus.value_xy.elevatorId = 2;
        sigToModbus.value_xy.boxId = background_askBoxDownByBtn_boxDownId_;

        emit sig_modbusmanager_orders(sigToModbus);
        break;
    }
    case SIG_MODBUSMANAGEMENT_MAINWINDOW_XY_OUT:
    {
        Sig_Mainwindown_Modbusmanagement sigToModbus;
        sigToModbus.type = SIG_MAINWINDOWN_MODBUSMANAGEMENT_ELEVATOR_DOWN;
        sigToModbus.value_elevator.elevatorId = 2;

        emit sig_modbusmanager_orders(sigToModbus);
        break;
    }
    case SIG_MODBUSMANAGEMENT_MAINWINDOW_XY_IN:
    {
        ui->btn_background_askBoxDownByBtn_store->setDisabled(true);
        break;
    }
    case SIG_MODBUSMANAGEMENT_MAINWINDOW_MODBUS_ERRORS:
    {
        //todo //show message in label
        QString errors;
        for(int i=0;i<sig.value_modbusErrors.errors.size();i++)
        {
            errors += sig.value_modbusErrors.errors[i]+",";
        }
        ui->label_mainwindow_modbus_errors->setText(errors);
        break;
    }
    case SIG_MODBUSMANAGEMENT_MAINWINDOW_MODBUS_FIXED:
    {
        //todo //clear message from label
        ui->label_mainwindow_modbus_errors->clear();
        break;
    }
    case SIG_MODBUSMANAGEMENT_MAINWINDOW_ORDER_TIMEOUT:
    {
        switch (ui->stackedWidget->currentIndex()) {
        case STACK_WIDGET_PAGE_STORE_MULTI_INDEX:
        {
            //todo
            label_box_moving_->hide();
            break;
        }
        case STACK_WIDGET_PAGE_TAKE_CUSTOMER_TAKEPAGE_INDEX:
        {
            //todo
            label_box_moving_->hide();
            break;
        }
        default:
        {
            qDebug()<<"unknown page";
            break;
        }
        }
        ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_HOMENEW_INDEX);
        break;
    }
    default:
    {
        qDebug()<<"unknow sig from modbus";
        break;
    }
    }
}

void MainWindow::slot_realsence(Sig_RealsenceManagement_MainWindow sig)
{
    switch (sig.type) {
    case SIG_REALSENCEMANAGEMENT_MAINWINDOW_BOXEMPTYVOLUME_ELEVATOR2:
    {
        Sig_Mainwindow_Boxmanagement sigToBox;
        sigToBox.type = SIG_MAINWINDOW_BOXMANAGEMENT_UPDATE_BOXVOLUME;
        sigToBox.value_updateBoxVolume.boxId = background_askBoxDownByBtn_boxDownId_;
        sigToBox.value_updateBoxVolume.volume = REALSENSE_BOX_FULLVOLUME - sig.value_BoxEmptyVolume_elevator2.volume;
        emit sig_boxManager(sigToBox);

        Sig_Mainwindown_Modbusmanagement sigToModbus;
        sigToModbus.type = SIG_MAINWINDOWN_MODBUSMANAGEMENT_ELEVATOR_UP;
        sigToModbus.value_elevator.elevatorId = 2;
        emit sig_modbusmanager_orders(sigToModbus);
        break;
    }
    default:
    {
        qDebug()<<"unknows sig from realsence";
        break;
    }
    }
}

void MainWindow::show_ItemsInfoSearchedByDay(QVector<Item_FamilyMart> vec_item)
{
    count_table_take_servenday_model_selected_ = 0;
    table_take_servenday_map_row_item_.clear();
    table_take_servenday_model_->setRowCount(0);

    auto func_countDate = [&](QString date)
    {
        int result = -1;
        QDate today = QDate::currentDate();
//        qDebug()<<today;
//        qDebug()<<today.toString();

        QStringList dateList = date.split("/");
        if(dateList.size()!=3)
        {
            qDebug()<<"date error";
        }
        else
        {
            result = -today.daysTo(QDate(dateList[0].toInt(),dateList[1].toInt(),dateList[2].toInt()));
        }
        return result;
    };

//    QString date("2021/8/4");
//    func_countDate(date);
    qDebug()<<"get item by day:"<<vec_item.size();
    for(int i=0;i<vec_item.size();i++)
    {
        table_take_servenday_model_->setItem(i,0,new QStandardItem(vec_item[i].infos.name));
        table_take_servenday_model_->setItem(i,1,new QStandardItem(vec_item[i].infos.phone_number));

        QString day_arrive = QString::number(func_countDate(vec_item[i].infos.date_arrive));
        table_take_servenday_model_->setItem(i,2,new QStandardItem(day_arrive));

        QString date_arrive;
        QStringList data_arrive_list = vec_item[i].infos.date_arrive.split('/');
        if(data_arrive_list.size()!=3)
        {
            qDebug()<<"date error";
        }
        else
        {
            if(vec_item[i].infos.weekday.isEmpty())
            {
                switch(QDate(data_arrive_list[0].toInt(),data_arrive_list[1].toInt(),data_arrive_list[2].toInt()).dayOfWeek())
                {
                case 1:{
                    vec_item[i].infos.weekday = "一";
                    break;
                }
                case 2:{
                    vec_item[i].infos.weekday = "二";
                    break;
                }
                case 3:{
                    vec_item[i].infos.weekday = "三";
                    break;
                }
                case 4:{
                    vec_item[i].infos.weekday = "四";
                    break;
                }
                case 5:{
                    vec_item[i].infos.weekday = "五";
                    break;
                }
                case 6:{
                    vec_item[i].infos.weekday = "六";
                    break;
                }
                case 7:{
                    vec_item[i].infos.weekday = "七";
                    break;
                }
                }
            }
            date_arrive = data_arrive_list[1]+"/"+data_arrive_list[2]+"("+vec_item[i].infos.weekday+")";
        }

        table_take_servenday_model_->setItem(i,3,new QStandardItem(date_arrive));
//        table_take_servenday_model_->setItem(i,4,new QStandardItem(vec_item[i].infos.weekday));
        table_take_servenday_model_->setItem(i,4,new QStandardItem(tr("✓")));
        table_take_servenday_model_->item(i,0)->setTextAlignment(Qt::AlignCenter);
        table_take_servenday_model_->item(i,1)->setTextAlignment(Qt::AlignCenter);
        table_take_servenday_model_->item(i,2)->setTextAlignment(Qt::AlignCenter);
        table_take_servenday_model_->item(i,3)->setTextAlignment(Qt::AlignCenter);
        table_take_servenday_model_->item(i,4)->setTextAlignment(Qt::AlignCenter);

        ui->table_servenday_list->resizeRowToContents(i);

        table_take_servenday_map_row_item_.insert(i,vec_item[i]);
        count_table_take_servenday_model_selected_++;
    }

    ui->btn_take_sevenday_take->setText(QString(tr("%1 筆出庫").arg(count_table_take_servenday_model_selected_)));
}

void MainWindow::on_table_servenday_list_clicked(const QModelIndex &index)
{
    if(index.isValid())
    {
        if(index.column() == 4)
        {
            if(table_take_servenday_model_->itemData(index).value(0).toString() ==tr("口"))
            {
                table_take_servenday_model_->setItem(index.row(),4,new QStandardItem(tr("✓")));
                table_take_servenday_model_->item(index.row(),4)->setTextAlignment(Qt::AlignCenter);

                count_table_take_servenday_model_selected_++;

                ui->btn_take_sevenday_take->setText(QString(tr("%1 筆出庫").arg(count_table_take_servenday_model_selected_)));
            }
            else if(table_take_servenday_model_->itemData(index).value(0).toString() == tr("✓"))
            {
                table_take_servenday_model_->setItem(index.row(),4,new QStandardItem("口"));
                table_take_servenday_model_->item(index.row(),4)->setTextAlignment(Qt::AlignCenter);
                count_table_take_servenday_model_selected_--;
                ui->btn_take_sevenday_take->setText(QString(tr("%1 筆出庫").arg(count_table_take_servenday_model_selected_)));
            }
        }

    }
}

void MainWindow::slot_clipboardmanager_nameImage(QImage img)
{
//    nameImage_ = img;

//    int currentPage = ui->stackedWidget->currentIndex();
//    switch (currentPage) {
//    case STACK_WIDGET_PAGE_STORE_MULTI_INDEX:
//    {
//        ui->label_store_multi_name->clear();
//        img = img.scaled(200,70);
//        ui->label_store_multi_name->setPixmap(QPixmap::fromImage(img));
//        ui->label_store_multi_name->resize(img.width(),img.height());
//        break;
//    }
//    case STACK_WIDGET_PAGE_STORE_SINGLE_INDEX:
//    {
//        ui->label_store_single_name->clear();
//        img = img.scaled(200,100);
//        ui->label_store_single_name->setPixmap(QPixmap::fromImage(img));
//        ui->label_store_single_name->resize(img.width(),img.height());
//        break;
//    }
//    }
}



void MainWindow::on_btn_store_single_name_takePicAgain_clicked()
{
    emit sig_clipboardmanager_askNameImage();
}

void MainWindow::on_btn_store_multi_takeNamePicAgain_clicked()
{
    emit sig_clipboardmanager_askNameImage();
}

void MainWindow::on_btn_background_detectEmptyBox_clicked()
{
    ui->btn_background_back->setDisabled(true);
    ui->btn_background_searchItem->setDisabled(true);
    ui->btn_background_detectEmptyBox->setDisabled(true);
//    emit sig_jobmanager_detectEmptyBox();

    Sig_Mainwindow_Jobmanagement sigToJob;
    sigToJob.type = SIG_MAINWINDOWN_JOBMANAGEMENT_DETECT_EMPTYBOXVOLUME_VOLUME_ELEVATOR1;
    emit sig_jobmanager(sigToJob);
}

void MainWindow::on_btn_home_title_clicked()
{
    ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_BACKGROUD_INDEX);

    //show all items in table
    Sig_Mainwindow_Itemmanagement sigToItem;
    sigToItem.type = SIG_MAINWINDOWN_ITEMMANAGEMENT_ASKITEMS_ALL;
    emit sig_itemmanager(sigToItem);
}

void MainWindow::on_btn_background_back_clicked()
{
    ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_HOMENEW_INDEX);
}

void MainWindow::on_btn_homeNew_customerTake_clicked()
{
    //ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_TAKE_CUSTOMER_INDEX);
    on_btn_takehome_customer_clicked();
}

void MainWindow::on_btn_homeNew_multiStore_clicked()
{
    //ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_STORE_MULTI_INDEX);
    on_btn_store_multistore_clicked();
}

void MainWindow::on_btn_homeNew_sevendayTake_clicked()
{
    //ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_SEVENDAY_INDEX);
    on_btn_takehome_sevenday_clicked();
}

void MainWindow::on_btn_homeNew_logo_clicked()
{
    on_btn_home_title_clicked();
}

void MainWindow::on_btn_background_plc_recovery_clicked()
{
//    emit sig_jobmanager_modbus_plc_recovery();
    ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_BACKGROUND_PLC_INDEX);
}

void MainWindow::on_btn_background_plc_recovery_2_clicked()
{
    Sig_Mainwindow_Jobmanagement sig;
    sig.type = SIG_MAINWINDOWN_JOBMANAGEMENT_PLC_RECOVERY;

    emit sig_jobmanager(sig);
}

void MainWindow::on_btn_background_plc_putIn_clicked()
{
    Sig_Mainwindow_Jobmanagement sig;
    sig.type = SIG_MAINWINDOW_JOBMANAGEMENT_MODBUS_PUTIN;
    sig.value_putIn.box_id = ui->lineEdit_background_plc_boxNumber->text().toInt();
    sig.value_putIn.elevator_id = ui->lineEdit_background_plc_elevatorId->text().toInt();

    emit sig_jobmanager(sig);
}

void MainWindow::on_btn_background_plc_out_clicked()
{
//    Sig_Mainwindow_Jobmanagement sig;
//    sig.type = SIG_MAINWINDOW_JOBMANAGEMENT_MODBUS_DIRECT_OUT;
////    sig.value_direct_out.box_id = ui->lineEdit_background_plc_boxNumber->text().toInt();
//    sig.value_direct_out.elevator_id = ui->lineEdit_background_plc_elevatorId->text().toInt();

    Sig_Mainwindow_Jobmanagement sig;
    sig.type = SIG_MAINWINDOW_JOBMANAGEMENT_MODBUS_OUT;
    sig.value_out.box_id = ui->lineEdit_background_plc_boxNumber->text().toInt();
    sig.value_out.elevator_id = ui->lineEdit_background_plc_elevatorId->text().toInt();

    emit sig_jobmanager(sig);
}

void MainWindow::on_btn_background_plc_back_clicked()
{
    ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_HOMENEW_INDEX);
}

void MainWindow::on_btn_background_test_clicked()
{
    emit sig_realsencemanager_test();
}

void MainWindow::on_btn_background_detect_volume_clicked()
{
    Sig_Mainwindow_Jobmanagement sigToJob;
    sigToJob.type = SIG_MAINWINDOWN_JOBMANAGEMENT_DETECT_EMPTYBOXVOLUME_VOLUME_ELEVATOR2;
    emit sig_jobmanager(sigToJob);
}


void MainWindow::on_btn_background_plc_XY_putIn_clicked()
{
    int boxId = ui->lineEdit_background_plc_boxNumber->text().toInt();
    int elevatorId = ui->lineEdit_background_plc_elevatorId->text().toInt();

    Sig_Mainwindown_Modbusmanagement sig;
    sig.type = SIG_MAINWINDOWN_MODBUSMANAGEMENT_XY_PUTIN;
    sig.value_xy.boxId = boxId;
    sig.value_xy.elevatorId = elevatorId;
    emit sig_modbusmanager_orders(sig);

    Sig_Mainwindow_Jobmanagement sigToJob;
    sigToJob.type = SIG_MAINWINDOWN_JOBMANAGEMENT_INIT_MODBUS_XYIN;
    sigToJob.value_Init_Modbus_XyIn.box_id = boxId;
    sigToJob.value_Init_Modbus_XyIn.elevator_id = elevatorId;
    emit sig_jobmanager(sigToJob);
}

void MainWindow::on_btn_background_plc_XY_out_clicked()
{
    int boxId = ui->lineEdit_background_plc_boxNumber->text().toInt();
    int elevatorId = ui->lineEdit_background_plc_elevatorId->text().toInt();

    Sig_Mainwindown_Modbusmanagement sig;
    sig.type = SIG_MAINWINDOWN_MODBUSMANAGEMENT_XY_OUT;
    sig.value_xy.boxId = boxId;
    sig.value_xy.elevatorId = elevatorId;
    emit sig_modbusmanager_orders(sig);

    Sig_Mainwindow_Jobmanagement sigToJob;
    sigToJob.type = SIG_MAINWINDOWN_JOBMANAGEMENT_INIT_MODBUS_XYOUT;
    sigToJob.value_Init_Modbus_XyOut.box_id = boxId;
    sigToJob.value_Init_Modbus_XyOut.elevator_id = elevatorId;
    emit sig_jobmanager(sigToJob);
}

void MainWindow::on_btn_background_plc_elevator_up_clicked()
{
//    int boxId = ui->lineEdit_background_plc_boxNumber->text().toInt();
    int elevatorId = ui->lineEdit_background_plc_elevatorId->text().toInt();

    Sig_Mainwindown_Modbusmanagement sig;
    sig.type = SIG_MAINWINDOWN_MODBUSMANAGEMENT_ELEVATOR_UP;
    sig.value_elevator.elevatorId = elevatorId;
    emit sig_modbusmanager_orders(sig);

    Sig_Mainwindow_Jobmanagement sigToJob;
    sigToJob.type = SIG_MAINWINDOWN_JOBMANAGEMENT_INIT_MODBUS_ELEVATOR_UP;
    sigToJob.value_Init_Modbus_ElevatorUp.elevator_id = elevatorId;
    emit sig_jobmanager(sigToJob);
}

void MainWindow::on_btn_background_plc_elevator_down_clicked()
{
    int elevatorId = ui->lineEdit_background_plc_elevatorId->text().toInt();

    Sig_Mainwindown_Modbusmanagement sig;
    sig.type = SIG_MAINWINDOWN_MODBUSMANAGEMENT_ELEVATOR_DOWN;
    sig.value_elevator.elevatorId = elevatorId;
    emit sig_modbusmanager_orders(sig);

    Sig_Mainwindow_Jobmanagement sigToJob;
    sigToJob.type = SIG_MAINWINDOWN_JOBMANAGEMENT_INIT_MODBUS_ELEVATOR_DOWN;
    sigToJob.value_Init_Modbus_ElevatorDown.elevator_id = elevatorId;
    emit sig_jobmanager(sigToJob);
}

void MainWindow::askItemsByNumber(QString number)
{
    Sig_Mainwindow_Itemmanagement sig;
    sig.type = SIG_MAINWINDOWN_ITEMMANAGEMENT_ASKITEMS_BYNUMBER;
    sig.value_askItemsByNumber.number = number;

    emit sig_itemmanager(sig);
}
void MainWindow::askItemsByBeyongdDay(int day)
{
    Sig_Mainwindow_Itemmanagement sig;
    sig.type = SIG_MAINWINDOWN_ITEMMANAGEMENT_ASKITEMS_BYBEYONDDAY;
    sig.value_askItemsByBeyondDay.day =day;

    emit sig_itemmanager(sig);
}

void MainWindow::askItemByBarcode(QString barcode)
{
    Sig_Mainwindow_Itemmanagement sig;
    sig.type = SIG_MAINWINDOWN_ITEMMANAGEMENT_ASKITEM_BYBARCODE;
    sig.value_askItemByBarcode.barcode = barcode;
    emit sig_itemmanager(sig);
}

void MainWindow::askAllItems()
{
    Sig_Mainwindow_Itemmanagement sig;
    sig.type = SIG_MAINWINDOWN_ITEMMANAGEMENT_ASKITEMS_ALL;

    emit sig_itemmanager(sig);
}

void MainWindow::askBoxDownByItems(QVector<Item_FamilyMart> items)
{
    Sig_Mainwindow_Jobmanagement sig;
    sig.type = SIG_MAINWINDOWN_JOBMANAGEMENT_ASK_BOXDOWN_BYITEMS;
    sig.value_askBoxDownByItems.items = items;

    emit sig_jobmanager(sig);
}

void MainWindow::askBoxDownMostEmpty()
{
    Sig_Mainwindow_Jobmanagement sig;
    sig.type = SIG_MAINWINDOWN_JOBMANAGEMENT_ASK_BOXDOWN_MOSTEMPTY;

    emit sig_jobmanager(sig);
}

void MainWindow::boxDown(int boxId)
{
    switch (ui->stackedWidget->currentIndex()) {
    case STACK_WIDGET_PAGE_STORE_MULTI_INDEX:
    {
        qDebug()<<"box is down";
        label_box_moving_->hide();
        ui->btn_store_multi_store->setEnabled(true);
        ui->btn_store_multi_changeBox->setEnabled(true);
//        label_box_moving_->setText(tr("請放入物品"));


        //enable barcode scaner
//        Sig_Mainwindow_Clipmanagement sigToClip;
//        sigToClip.type = SIG_MAINWINDOW_CLIPMANAGEMENT_LISTEN_BARCODE;
//        emit sig_clipboard(sigToClip);
        this->setFocus();
        break;
    }
    case STACK_WIDGET_PAGE_TAKE_CUSTOMER_TAKEPAGE_INDEX:
    {

        if(STACK_WIDGET_PAGE_TAKE_CUSTOMER_INDEX == pageIndex_prepage_)
        {
            //todo get pre page's item and show with boxId
            QList<Item_FamilyMart> itemToTake = table_take_customer_map_row_item_.values();
            QVector<Item_FamilyMart> vec_item;
            for(int i=0;i<itemToTake.size();i++)
            {
                if(boxId == itemToTake[i].box.box_id)
                {
                    vec_item.append(itemToTake[i]);
                }
            }
            // show in takepage
            show_ItemsInfo_takePage(vec_item);
            //if [take finish] need to delete item
            //store total item info
            items_takepage_ = vec_item;
        }
        else if(STACK_WIDGET_PAGE_SEVENDAY_INDEX == pageIndex_prepage_)
        {
            //todo get pre page's item and show with boxId
            QList<Item_FamilyMart> itemToTake = table_take_servenday_map_row_item_.values();
            QVector<Item_FamilyMart> vec_item;
            for(int i=0;i<itemToTake.size();i++)
            {
                if(boxId == itemToTake[i].box.box_id)
                {
                    vec_item.append(itemToTake[i]);
                }
            }
            //todo show in takepage
            show_ItemsInfo_takePage(vec_item);
            //if [take finish] need to delete item
            //store total item info
            items_takepage_ = vec_item;
        }
        else
        {
            qDebug()<<" MainWindow::boxDown : unknown pre page";
        }
//        qDebug()<<"window: get a box down"<<boxId;
//        Sig_Mainwindow_Itemmanagement sigToItem;
//        sigToItem.type = SIG_MAINWINDOWN_ITEMMANAGEMENT_ASKITEMS_BYBOXID;
//        sigToItem.value_askItemsByBoxId.boxId = boxId;
//        qDebug()<<"send sig to item:"<<SIG_MAINWINDOWN_ITEMMANAGEMENT_ASKITEMS_BYBOXID;
//        emit sig_itemmanager(sigToItem);

        label_box_moving_->hide();
        ui->btn_take_takepage_finish->setEnabled(true);
        ui->btn_take_takepage_cancel->setEnabled(true);
        break;
    }
    case STACK_WIDGET_PAGE_BACKGROUND_PLC_INDEX:
    {
        break;
    }
    default:
    {
        qDebug()<<"MainWindow::boxDown : known signal";
        break;
    }
    }
}

void MainWindow::boxInFinished()
{
    if(ui->stackedWidget->currentIndex() == STACK_WIDGET_PAGE_STORE_MULTI_INDEX)
    {
        if(flag_multi_store_changeBox)
        {
            Sig_Mainwindow_Jobmanagement sigToJob;
            sigToJob.type = SIG_MAINWINDOWN_JOBMANAGEMENT_ASK_BOXDOWN_MOSTEMPTY;
            emit sig_jobmanager(sigToJob);
        }
        else
        {
            label_box_moving_->hide();
            items_takepage_.clear();
            ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_HOMENEW_INDEX);
        }
    }
    else
    {
        label_box_moving_->hide();
        items_takepage_.clear();
        ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_HOMENEW_INDEX);
    }
}

void MainWindow::detect_emptyBoxVolume()
{
    Sig_Mainwindow_Jobmanagement sig;
    sig.type = SIG_MAINWINDOWN_JOBMANAGEMENT_DETECT_EMPTYBOXVOLUME_VOLUME_ELEVATOR1;

    emit sig_jobmanager(sig);
}

void MainWindow::ask_plc_recovery()
{
    Sig_Mainwindow_Jobmanagement sig;
    sig.type = SIG_MAINWINDOWN_JOBMANAGEMENT_PLC_RECOVERY;

    emit sig_jobmanager(sig);
}

void MainWindow::multiStore_getBarcode(QString barcode)
{
    Sig_Mainwindow_Itemmanagement sigToItem;
    sigToItem.type = SIG_MAINWINDOWN_ITEMMANAGEMENT_ASKITEM_BYBARCODE;
    sigToItem.value_askItemByBarcode.barcode = barcode;

    emit sig_itemmanager(sigToItem);
}

void MainWindow::askBoxUpByItems(QVector<Item_FamilyMart> items)
{
    Sig_Mainwindow_Jobmanagement sig;
    sig.type = SIG_MAINWINDOWN_JOBMANAGEMENT_ASK_BOXUP;
    emit sig_jobmanager(sig);

    Sig_Mainwindow_Itemmanagement sigToItem;
    sigToItem.type = SIG_MAINWINDOWN_ITEMMANAGEMENT_ADDITEMS;
    sigToItem.value_addItems.items = items;
    emit sig_itemmanager(sigToItem);
}

//void MainWindow::keyPressEvent(QKeyEvent  *event)
//{
//    if(STACK_WIDGET_PAGE_STORE_MULTI_INDEX == ui->stackedWidget->currentIndex())
//    {
//        barcode_+= event->text();
//        qDebug()<<"barcode:"<<barcode_;
//        if(barcode_.size() == BARCODE_SIZE)
//        {
//    //        qDebug()<<barcode_;
//            if(ui->stackedWidget->currentIndex() == STACK_WIDGET_PAGE_STORE_MULTI_INDEX)
//            {
//                askItemByBarcode(barcode_);
//            }
//            barcode_.clear();
//        }
//    }
//}

// need to test
void MainWindow::keyPressEvent(QKeyEvent  *event)
{
    if(STACK_WIDGET_PAGE_STORE_MULTI_INDEX == ui->stackedWidget->currentIndex())
    {
        if(QString("\r") ==  event->text())
        {
            if(9 == tmp_barcode_.size())
            {
                barcode_nine_ = tmp_barcode_;
                tmp_barcode_.clear();
            }
            else if(16 == tmp_barcode_.size())
            {
                barcode_sixteen_ = tmp_barcode_;
                tmp_barcode_.clear();
            }
            else
            {
                tmp_barcode_.clear();
                return;
            }

            if(9 == barcode_nine_.size()&& 16 == barcode_sixteen_.size())
            {
                if(STACK_WIDGET_PAGE_STORE_MULTI_INDEX == ui->stackedWidget->currentIndex())
                {
                    askItemByBarcode(barcode_nine_+barcode_sixteen_);
                }
                else if(STACK_WIDGET_PAGE_TAKE_CUSTOMER_TAKEPAGE_INDEX == ui->stackedWidget->currentIndex())
                {
                    //todo: select the row by barcode
                    takepage_selectRowByBarcode(barcode_nine_+barcode_sixteen_);
                }

                barcode_nine_.clear();
                barcode_sixteen_.clear();
            }
        }
        else
        {
            tmp_barcode_+= event->text();
            qDebug()<<tmp_barcode_;
        }
    }
}


void MainWindow::on_btn_background_takeE1Volume_clicked()
{
    Sig_MainWindow_RealsenceManagement sig;
    sig.type = SIG_MAINWINDOW_REALSENCEMANAGEMENT_TAKE_BOXLEFTVOLUME_ELEVATOR1;
    emit sig_realsencemanager_test_format(sig);
}

void MainWindow::on_btn_background_takeE2Volume_clicked()
{
    Sig_MainWindow_RealsenceManagement sig;
    sig.type = SIG_MAINWINDOW_REALSENCEMANAGEMENT_TAKE_BOXLEFTVOLUME_ELEVATOR2;
    emit sig_realsencemanager_test_format(sig);
}

void MainWindow::on_btn_background_quitProgram_clicked()
{
    emit sig_quitProgram();
}

void MainWindow::on_btn_store_multi_changeBox_clicked()
{
    //get item data from tableview
    vec_tmp_item_.clear();
    QVector<Item_FamilyMart> vec_leftItems_forChangeBox;
    for(int i_row = 0;i_row < table_store_multi_model_->rowCount();i_row++)
    {
        QString flag = table_store_multi_model_->item(i_row,2)->text();
        if(tr("✓") == flag)
        {
            QString name = table_store_multi_model_->item(i_row,0)->text();
            QString number = table_store_multi_model_->item(i_row,1)->text();
            QString barcode = table_store_multi_model_->item(i_row,2)->text();

            Item_FamilyMart item;
            item.infos.name = name;
            item.infos.phone_number = number;
            item.infos.barcode = barcode;

            vec_tmp_item_.push_back(item);
        }
        else
        {
            QString name = table_store_multi_model_->item(i_row,0)->text();
            QString number = table_store_multi_model_->item(i_row,1)->text();
            QString barcode = table_store_multi_model_->item(i_row,2)->text();

            Item_FamilyMart item;
            item.infos.name = name;
            item.infos.phone_number = number;
            item.infos.barcode = barcode;

            vec_leftItems_forChangeBox.push_back(item);
        }
    }
//    emit sig_jobmanager_ItemInBox(vec_tmp_item_);
    if(vec_tmp_item_.size()>0)
    {
        Sig_Mainwindow_Jobmanagement sigToJob_AddNewItems;
        sigToJob_AddNewItems.type = SIG_MAINWINDOWN_JOBMANAGEMENT_ADDNEWITEMS;
        sigToJob_AddNewItems.value_addNewItems.items = vec_tmp_item_;
        emit sig_jobmanager(sigToJob_AddNewItems);
    }
    else
    {
        Sig_Mainwindow_Jobmanagement sigToJob;
        sigToJob.type = SIG_MAINWINDOWN_JOBMANAGEMENT_ASK_BOXUP;
        emit sig_jobmanager(sigToJob);
    }


    table_store_multi_model_->setRowCount(0);
    count_item_selected_ = 0;


    label_box_moving_->show();
    ui->btn_store_multi_store->setDisabled(true);
    ui->btn_store_multi_changeBox->setDisabled(true);

    for(int i=0;i<vec_leftItems_forChangeBox.size();i++)
    {
        QString name = vec_leftItems_forChangeBox[i].infos.name;
        QString phoneNumber = vec_leftItems_forChangeBox[i].infos.phone_number;
        QString barcode = vec_leftItems_forChangeBox[i].infos.barcode;

        QList<QStandardItem *> table_items;
        QStandardItem *Item_name = new QStandardItem(name);
        Item_name->setTextAlignment(Qt::AlignCenter);
        table_items.append(Item_name);
        QStandardItem *Item_phone = new QStandardItem(phoneNumber);
        Item_phone->setTextAlignment(Qt::AlignCenter);
        table_items.append(Item_phone);

        QStandardItem *Item_barcode = new QStandardItem(barcode);
        Item_barcode->setTextAlignment(Qt::AlignCenter);
        table_items.append(Item_barcode);

        QStandardItem *Item_flag = new QStandardItem(tr("口"));
        Item_flag->setTextAlignment(Qt::AlignCenter);
        table_items.append(Item_flag);
        table_store_multi_model_->appendRow(table_items);

        ui->tableView_store_multi->resizeRowsToContents();
    }
    flag_multi_store_changeBox = true;
}



void MainWindow::on_tableView_store_multi_doubleClicked(const QModelIndex &index)
{
    if(ui->tableView_store_multi->hasFocus())
    {
        ui->tableView_store_multi->clearFocus();
    }
}

void MainWindow::on_btn_background_searchItem_clicked()
{
    ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_ASKBOXDOWNBYBUTTON);
}

void MainWindow::on_btn_background_askBoxDownByBtn_back_clicked()
{
    ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_HOMENEW_INDEX);
}

QVector<Item_FamilyMart> MainWindow::sortItemsByNameAndPhone(QVector<Item_FamilyMart> items)
{
    //map by name+phone
    QVector<Item_FamilyMart> result;
    QMap<QString,QVector<Item_FamilyMart>> map_namePhone_items;
    for(int i=0;i<items.size();i++)
    {
        QString key = items[i].infos.name+items[i].infos.phone_number;
        if(map_namePhone_items.contains(key))
        {
            map_namePhone_items[key].append(items[i]);
        }
        else
        {
            QVector<Item_FamilyMart> tmpItems;
            tmpItems.append(items[i]);
            map_namePhone_items.insert(key,tmpItems);
        }
    }
    //get all items sorted by name and phone
    QMapIterator<QString, QVector<Item_FamilyMart>> it(map_namePhone_items);
    while (it.hasNext()) {
        it.next();
        for(int i=0;i<it.value().size();i++)
        {
            result.push_back(it.value()[i]);
        }
    }

    return result;
}
QVector<Item_FamilyMart> MainWindow::sortItemsByBoxId(QVector<Item_FamilyMart> items)
{
    //map by name+phone
    QVector<Item_FamilyMart> result;
    QMap<int,QVector<Item_FamilyMart>> map_boxId_items;
    for(int i=0;i<items.size();i++)
    {
        int key = items[i].box.box_id;
        if(map_boxId_items.contains(key))
        {
            map_boxId_items[key].append(items[i]);
        }
        else
        {
            QVector<Item_FamilyMart> tmpItems;
            tmpItems.append(items[i]);
            map_boxId_items.insert(key,tmpItems);
        }
    }
    //get all items sorted by name and phone
    QMapIterator<int, QVector<Item_FamilyMart>> it(map_boxId_items);
    while (it.hasNext()) {
        it.next();
        for(int i=0;i<it.value().size();i++)
        {
            result.push_back(it.value()[i]);
        }
    }

    return result;
}

void MainWindow::emitSigToModbusXyOut(int boxId,int elevatorId)
{
    qDebug()<<"ask box down : "<< boxId;
    Sig_Mainwindown_Modbusmanagement sig;
    sig.type = SIG_MAINWINDOWN_MODBUSMANAGEMENT_XY_OUT;
    sig.value_xy.boxId = boxId;
    sig.value_xy.elevatorId = elevatorId;
    emit sig_modbusmanager_orders(sig);

    background_askBoxDownByBtn_boxDownId_ = boxId;

    //set all button unusable
    ui->btn_background_askBoxDownByBtn_box1->setDisabled(true);
    ui->btn_background_askBoxDownByBtn_box2->setDisabled(true);
    ui->btn_background_askBoxDownByBtn_box3->setDisabled(true);
    ui->btn_background_askBoxDownByBtn_box4->setDisabled(true);
    ui->btn_background_askBoxDownByBtn_box5->setDisabled(true);
    ui->btn_background_askBoxDownByBtn_box6->setDisabled(true);
    ui->btn_background_askBoxDownByBtn_box7->setDisabled(true);
    ui->btn_background_askBoxDownByBtn_box8->setDisabled(true);
    ui->btn_background_askBoxDownByBtn_box9->setDisabled(true);
    ui->btn_background_askBoxDownByBtn_box10->setDisabled(true);
    ui->btn_background_askBoxDownByBtn_box11->setDisabled(true);
    ui->btn_background_askBoxDownByBtn_box12->setDisabled(true);
    ui->btn_background_askBoxDownByBtn_box13->setDisabled(true);
    ui->btn_background_askBoxDownByBtn_box14->setDisabled(true);
    ui->btn_background_askBoxDownByBtn_box15->setDisabled(true);
    ui->btn_background_askBoxDownByBtn_box16->setDisabled(true);
    ui->btn_background_askBoxDownByBtn_box17->setDisabled(true);
    ui->btn_background_askBoxDownByBtn_box18->setDisabled(true);
    ui->btn_background_askBoxDownByBtn_box19->setDisabled(true);
    ui->btn_background_askBoxDownByBtn_box20->setDisabled(true);
    ui->btn_background_askBoxDownByBtn_box21->setDisabled(true);
    ui->btn_background_askBoxDownByBtn_box22->setDisabled(true);
    ui->btn_background_askBoxDownByBtn_box23->setDisabled(true);
    ui->btn_background_askBoxDownByBtn_box24->setDisabled(true);
    ui->btn_background_askBoxDownByBtn_back->setDisabled(true);
}

void MainWindow::on_btn_background_askBoxDownByBtn_box1_clicked()
{
    emitSigToModbusXyOut(1,2);
}

void MainWindow::on_btn_background_askBoxDownByBtn_store_clicked()
{
//    Sig_Mainwindown_Modbusmanagement sig;
//    sig.type = SIG_MAINWINDOWN_MODBUSMANAGEMENT_BOX_PUTIN;
//    sig.value_boxIn.boxId = background_askBoxDownByBtn_boxDownId_;
//    sig.value_boxIn.elevatorId = 2;
//    emit sig_modbusmanager_orders(sig);
    Sig_MainWindow_RealsenceManagement sigToRealsence;
    sigToRealsence.type = SIG_MAINWINDOW_REALSENCEMANAGEMENT_TAKE_BOXLEFTVOLUME_ELEVATOR2;

    emit sig_realsencemanager_test_format(sigToRealsence);

    //set button enabled
    ui->btn_background_askBoxDownByBtn_back->setEnabled(true);
    ui->btn_background_askBoxDownByBtn_box1->setEnabled(true);
    ui->btn_background_askBoxDownByBtn_box2->setEnabled(true);
    ui->btn_background_askBoxDownByBtn_box3->setEnabled(true);
    ui->btn_background_askBoxDownByBtn_box4->setEnabled(true);
    ui->btn_background_askBoxDownByBtn_box5->setEnabled(true);
    ui->btn_background_askBoxDownByBtn_box6->setEnabled(true);
    ui->btn_background_askBoxDownByBtn_box7->setEnabled(true);
    ui->btn_background_askBoxDownByBtn_box8->setEnabled(true);
    ui->btn_background_askBoxDownByBtn_box9->setEnabled(true);
    ui->btn_background_askBoxDownByBtn_box10->setEnabled(true);
    ui->btn_background_askBoxDownByBtn_box11->setEnabled(true);
    ui->btn_background_askBoxDownByBtn_box12->setEnabled(true);
    ui->btn_background_askBoxDownByBtn_box13->setEnabled(true);
    ui->btn_background_askBoxDownByBtn_box14->setEnabled(true);
    ui->btn_background_askBoxDownByBtn_box15->setEnabled(true);
    ui->btn_background_askBoxDownByBtn_box16->setEnabled(true);
    ui->btn_background_askBoxDownByBtn_box17->setEnabled(true);
    ui->btn_background_askBoxDownByBtn_box18->setEnabled(true);
    ui->btn_background_askBoxDownByBtn_box19->setEnabled(true);
    ui->btn_background_askBoxDownByBtn_box20->setEnabled(true);
    ui->btn_background_askBoxDownByBtn_box21->setEnabled(true);
    ui->btn_background_askBoxDownByBtn_box22->setEnabled(true);
    ui->btn_background_askBoxDownByBtn_box23->setEnabled(true);
    ui->btn_background_askBoxDownByBtn_box24->setEnabled(true);
}

void MainWindow::on_btn_background_askBoxDownByBtn_box2_clicked()
{
    emitSigToModbusXyOut(2,2);
}

void MainWindow::on_btn_background_askBoxDownByBtn_box3_clicked()
{
    emitSigToModbusXyOut(3,2);
}

void MainWindow::on_btn_background_askBoxDownByBtn_box4_clicked()
{
    emitSigToModbusXyOut(4,2);
}

void MainWindow::on_btn_background_askBoxDownByBtn_box5_clicked()
{
    emitSigToModbusXyOut(5,2);
}

void MainWindow::on_btn_background_askBoxDownByBtn_box6_clicked()
{
    emitSigToModbusXyOut(6,2);
}

void MainWindow::on_btn_background_askBoxDownByBtn_box7_clicked()
{
    emitSigToModbusXyOut(7,2);
}

void MainWindow::on_btn_background_askBoxDownByBtn_box8_clicked()
{
    emitSigToModbusXyOut(8,2);
}

void MainWindow::on_btn_background_askBoxDownByBtn_box9_clicked()
{
    emitSigToModbusXyOut(9,2);
}

void MainWindow::on_btn_background_askBoxDownByBtn_box10_clicked()
{
    emitSigToModbusXyOut(10,2);
}

void MainWindow::on_btn_background_askBoxDownByBtn_box11_clicked()
{
    emitSigToModbusXyOut(11,2);
}

void MainWindow::on_btn_background_askBoxDownByBtn_box12_clicked()
{
    emitSigToModbusXyOut(12,2);
}

void MainWindow::on_btn_background_askBoxDownByBtn_box13_clicked()
{
    emitSigToModbusXyOut(13,2);
}

void MainWindow::on_btn_background_askBoxDownByBtn_box14_clicked()
{
    emitSigToModbusXyOut(14,2);
}

void MainWindow::on_btn_background_askBoxDownByBtn_box15_clicked()
{
    emitSigToModbusXyOut(15,2);
}

void MainWindow::on_btn_background_askBoxDownByBtn_box16_clicked()
{
    emitSigToModbusXyOut(16,2);
}

void MainWindow::on_btn_background_askBoxDownByBtn_box17_clicked()
{
    emitSigToModbusXyOut(17,2);
}

void MainWindow::on_btn_background_askBoxDownByBtn_box18_clicked()
{
    emitSigToModbusXyOut(18,2);
}

void MainWindow::on_btn_background_askBoxDownByBtn_box19_clicked()
{
    emitSigToModbusXyOut(19,2);
}

void MainWindow::on_btn_background_askBoxDownByBtn_box20_clicked()
{
    emitSigToModbusXyOut(20,2);
}

void MainWindow::on_btn_background_askBoxDownByBtn_box21_clicked()
{
    emitSigToModbusXyOut(21,2);
}

void MainWindow::on_btn_background_askBoxDownByBtn_box22_clicked()
{
    emitSigToModbusXyOut(22,2);
}

void MainWindow::on_btn_background_askBoxDownByBtn_box23_clicked()
{
    emitSigToModbusXyOut(23,2);
}

void MainWindow::on_btn_background_askBoxDownByBtn_box24_clicked()
{
    emitSigToModbusXyOut(24,2);
}

void MainWindow::on_btn_background_searchItemsByBoxId_clicked()
{
    ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_BACKGROUD_SHOWITEMSBYBOXID_INDEX);

    Sig_Mainwindow_Itemmanagement sigToItem;
    sigToItem.type = SIG_MAINWINDOWN_ITEMMANAGEMENT_ASKITEMS_ALL;

    emit sig_itemmanager(sigToItem);
}

void MainWindow::on_btn_background_showItemsByBoxId_back_clicked()
{
    ui->stackedWidget->setCurrentIndex(STACK_WIDGET_PAGE_HOMENEW_INDEX);
}


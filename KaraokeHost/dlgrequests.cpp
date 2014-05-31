#include "dlgrequests.h"
#include "ui_dlgrequests.h"
#include <QMenu>
#include <QMessageBox>
#include "khsettings.h"

extern KhSettings *settings;

DlgRequests::DlgRequests(KhSongs *fullData, RotationTableModel *rotationModel, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgRequests)
{
    ui->setupUi(this);
    requestsModel = new RequestsTableModel(this);
    ui->treeViewRequests->setModel(requestsModel);
    ui->treeViewRequests->header()->setSectionResizeMode(5,QHeaderView::Fixed);
    ui->treeViewRequests->header()->resizeSection(5,22);
    ui->treeViewRequests->hideColumn(0);
//    ui->treeViewRequests->header()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
//    ui->treeViewRequests->header()->setSectionResizeMode(2,QHeaderView::Stretch);
//    ui->treeViewRequests->header()->setSectionResizeMode(3,QHeaderView::Stretch);
//    ui->treeViewRequests->header()->setSectionResizeMode(4,QHeaderView::ResizeToContents);
//    ui->treeViewRequests->header()->setSectionResizeMode(1,QHeaderView::Interactive);
//    ui->treeViewRequests->header()->setSectionResizeMode(2,QHeaderView::Interactive);
//    ui->treeViewRequests->header()->setSectionResizeMode(3,QHeaderView::Interactive);
    connect(requestsModel, SIGNAL(layoutChanged()), this, SLOT(requestsModified()));
    songDbModel = new SongDBTableModel(this);
    songDbModel->setFullData(fullData);
    ui->treeViewSearch->setModel(songDbModel);
    ui->treeViewSearch->header()->setSectionResizeMode(3,QHeaderView::Fixed);
    ui->treeViewSearch->header()->resizeSection(3,60);
    cdgPreviewDialog = new DlgCdgPreview(this);
    connect(ui->treeViewRequests->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(requestSelectionChanged(QModelIndex,QModelIndex)));
    connect(ui->treeViewSearch->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(songSelectionChanged(QModelIndex,QModelIndex)));
    m_rotationModel = rotationModel;
    ui->comboBoxAddPosition->setEnabled(false);
    ui->comboBoxSingers->setEnabled(true);
    ui->lineEditSingerName->setEnabled(false);
    ui->labelAddPos->setEnabled(false);
    QStringList posOptions;
    posOptions << "After current singer";
    posOptions << "Fair (full rotation)";
    posOptions << "Bottom of rotation";
    ui->comboBoxAddPosition->addItems(posOptions);
    ui->comboBoxAddPosition->setCurrentIndex(1);
    settings->restoreColumnWidths(ui->treeViewRequests);
    settings->restoreColumnWidths(ui->treeViewSearch);
}

DlgRequests::~DlgRequests()
{

    delete ui;
}

void DlgRequests::on_pushButtonClose_clicked()
{
    settings->saveColumnWidths(ui->treeViewRequests);
    settings->saveColumnWidths(ui->treeViewSearch);
    close();
}

void DlgRequests::requestsModified()
{
    if (requestsModel->count() > 0)
    {
        this->show();
        //ui->treeViewRequests->header()->resizeSections(QHeaderView::Stretch);
    }
}

void DlgRequests::on_pushButtonSearch_clicked()
{
    songDbModel->applyFilter(ui->lineEditSearch->text());
    //ui->treeViewSearch->header()->resizeSections(QHeaderView::Stretch);

}

void DlgRequests::on_lineEditSearch_returnPressed()
{
    songDbModel->applyFilter(ui->lineEditSearch->text());
    //ui->treeViewSearch->header()->resizeSections(QHeaderView::Stretch);

}

void DlgRequests::requestSelectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    qDebug() << "Current selection " << current.row();
    if ((current.isValid()) && (ui->treeViewRequests->selectionModel()->selectedIndexes().size() > 0))
    {
//        ui->comboBoxSingers->clear();
//        ui->comboBoxSingers->addItems(rotSingers->getSingerList());
//        QString filterStr = current.sibling(current.row(),2).data().toString() + " " + current.sibling(current.row(),3).data().toString();
//        songDbModel->applyFilter(filterStr);
//        ui->lineEditSearch->setText(filterStr);
//        ui->treeViewSearch->header()->resizeSections(QHeaderView::Stretch);
    }

}

void DlgRequests::songSelectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    if (current.isValid())
    {
//        if (ui->treeViewSearch->selectionModel()->selectedIndexes().size() > 0)
//            ui->groupBoxAddSong->setEnabled(true);
//        else
//            ui->groupBoxAddSong->setEnabled(false);
    }
}

void DlgRequests::on_radioButtonExistingSinger_toggled(bool checked)
{
    ui->comboBoxAddPosition->setEnabled(!checked);
    ui->comboBoxSingers->setEnabled(checked);
    ui->lineEditSingerName->setEnabled(!checked);
    ui->labelAddPos->setEnabled(!checked);
}

void DlgRequests::on_pushButtonClearReqs_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("Are you sure?");
    msgBox.setInformativeText("This action will clear all received requests. This operation can not be undone.");
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.addButton(QMessageBox::Cancel);
    QPushButton *yesButton = msgBox.addButton(QMessageBox::Yes);
    msgBox.exec();
    if (msgBox.clickedButton() == yesButton)
    {
        requestsModel->deleteAll();
    }
}

void DlgRequests::on_treeViewRequests_clicked(const QModelIndex &index)
{
    if (index.column() == 5)
    {
        requestsModel->deleteRequestId(index.sibling(index.row(),0).data().toInt());
    }
    else
    {
        ui->comboBoxSingers->clear();
        QString singerName = index.sibling(index.row(),1).data().toString();
        QStringList singers = m_rotationModel->getSingerList();
        ui->comboBoxSingers->addItems(singers);

        QString filterStr = index.sibling(index.row(),2).data().toString() + " " + index.sibling(index.row(),3).data().toString();
        songDbModel->applyFilter(filterStr);
        ui->lineEditSearch->setText(filterStr);
        ui->treeViewSearch->header()->resizeSections(QHeaderView::Stretch);
        ui->lineEditSingerName->setText(singerName);

        int s = -1;
        for (int i=0; i < singers.size(); i++)
        {
            if (singers.at(i).toLower() == singerName.toLower())
            {
                s = i;
                break;
            }
        }
        if (s != -1)
        {
            ui->comboBoxSingers->setCurrentIndex(s);
        }
    }
}

void DlgRequests::on_pushButtonAddSong_clicked()
{
    if (ui->treeViewRequests->selectionModel()->selectedIndexes().size() < 1)
        return;
    if (ui->treeViewSearch->selectionModel()->selectedIndexes().size() < 1)
        return;
    if (ui->radioButtonNewSinger->isChecked())
    {
        if (ui->lineEditSingerName->text() == "")
            return;
        else if (m_rotationModel->exists(ui->lineEditSingerName->text()))
            return;
        else
        {
            int songid = songDbModel->getRowSong(ui->treeViewSearch->selectionModel()->selectedIndexes().at(0).row())->ID;
            m_rotationModel->add(ui->lineEditSingerName->text());
            KhSinger *rotSinger = m_rotationModel->getSingerByName(ui->lineEditSingerName->text());
            if ((ui->comboBoxAddPosition->currentText() == "After current singer") && (m_rotationModel->getCurrent() != NULL))
            {
                if (m_rotationModel->getCurrent()->position() != m_rotationModel->size())
                    m_rotationModel->moveSinger(rotSinger->position(),m_rotationModel->getCurrent()->position() + 1);
            }
            else if ((ui->comboBoxAddPosition->currentText() == "Fair (full rotation)") && (m_rotationModel->getCurrent() != NULL))
            {
                if (m_rotationModel->getCurrent()->position() != 1)
                    m_rotationModel->moveSinger(rotSinger->position(), m_rotationModel->getCurrent()->position());
            }
            rotSinger->addSongAtEnd(songid);

        }
    }
    else if (ui->radioButtonExistingSinger->isChecked())
    {
        m_rotationModel->layoutAboutToBeChanged();
        int songid = songDbModel->getRowSong(ui->treeViewSearch->selectionModel()->selectedIndexes().at(0).row())->ID;
        m_rotationModel->getSingerByName(ui->comboBoxSingers->currentText())->addSongAtEnd(songid);
        m_rotationModel->layoutChanged();
    }
}

void DlgRequests::on_treeViewSearch_customContextMenuRequested(const QPoint &pos)
{
    qDebug() << "on_treeViewSearch_customContextMenuRequested fired";
    QModelIndex index = ui->treeViewSearch->indexAt(pos);
    if (index.isValid())
    {
        QString zipPath = songDbModel->getRowSong(index.row())->path;
        cdgPreviewDialog->setZipFile(zipPath);
        QMenu contextMenu(this);
        contextMenu.addAction("Preview", cdgPreviewDialog, SLOT(preview()));
        contextMenu.exec(QCursor::pos());
        //contextMenu->exec(ui->treeView->mapToGlobal(point));
    }
}

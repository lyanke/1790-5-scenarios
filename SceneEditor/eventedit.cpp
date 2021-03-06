#include "eventedit.h"
#include "editaction.h"
#include <QMessageBox>

eventEdit::eventEdit(QWidget *parent) :
    QDialog(parent)
{
	setupUi(this);
	setModal(true);
	grb_Pos->setHidden(true);
	grb_Time->setHidden(true);
}

eventEdit::eventEdit(QWidget *parent, QList<CEvent> *list, int index) :
	QDialog(parent),
	index(index),
	listeEvents(list)
{
	connect(this, SIGNAL(dataChanged()), SLOT(reloadList()));
	if(index != -1) {
		currentEvent = new CEvent((*list)[index]) ;
		setupUi(this);
		setModal(true);
		qle_Nom->setText(currentEvent->name());
		// Parametrage de l'interface
        if(currentEvent->declencheur()->type()=="manuel") {
			rdbManuel->setChecked(true);
			grb_Pos->setHidden(true);
			grb_Time->setHidden(true);
		}
		else if(currentEvent->declencheur()->type()=="time") {
			rdbTime->setChecked(true);
			grb_Pos->setHidden(true);
		}
		else if(currentEvent->declencheur()->type()=="position") {
			rdbPos->setChecked(true);
			grb_Time->setHidden(true);
			qle_Latitude->setText(((CPositionDeclencheur *)(currentEvent->declencheur()))->position().latitude());
			qle_Longitude->setText(((CPositionDeclencheur *)(currentEvent->declencheur()))->position().longitude());
			qle_Rayon->setText(((CPositionDeclencheur *)(currentEvent->declencheur()))->rayon());
		}
		pte_Description->appendPlainText(currentEvent->description());
		for(int i=0;i<currentEvent->actions().size();i++) {
			qlw_Actions->addItem(currentEvent->actions().at(i)->type() + " " + currentEvent->actions().at(i)->object().affichageName());
		}
	}
	else {
		setupUi(this);
		setModal(true);
		grb_Pos->setHidden(true);
		grb_Time->setHidden(true);
		currentEvent = new CEvent();
		rdbManuel->setChecked(true);
		qle_Nom->setText(currentEvent->name());
		pte_Description->appendPlainText(currentEvent->description());
	}
}

void eventEdit::on_pbuAddAction_clicked()
{
	editAction* edit = new editAction(this,&(currentEvent->actions()));
	edit->show();
	connect(edit, SIGNAL(finished(int)), this, SLOT(reloadList()));
}

void eventEdit::on_pbuEditAction_clicked()
{
	QModelIndex index = qlw_Actions->currentIndex();
	if(index.row()!=-1) {
		editAction *edit = new editAction(this, &(currentEvent->actions()), index.row());
		edit->show();
		connect(edit, SIGNAL(finished(int)), this, SIGNAL(dataChanged()));
	}
}

void eventEdit::on_pbuDeleteAction_clicked()
{
	QModelIndex index = qlw_Actions->currentIndex();
	if(index.row()!=-1) {
		QMessageBox::StandardButton reply;
		reply = QMessageBox::question(this, "Confirmation",
					"Êtes-vous sûr de vouloir supprimer cet élement? Attention, cette opération est définitive",
					QMessageBox::Yes|QMessageBox::No);
		if (reply == QMessageBox::Yes) {
			currentEvent->actions().removeAt(index.row());
			emit dataChanged();
		}
	}
}

void eventEdit::reloadList()
{
	qlw_Actions->clear();
	for(int i=0;i<currentEvent->actions().size();i++) {
		qlw_Actions->addItem(currentEvent->actions().at(i)->type() + " " + currentEvent->actions().at(i)->object().affichageName());
	}
}

void eventEdit::on_pbuValider_clicked()
{
	if(index != -1) {
		listeEvents->replace(index, *currentEvent);
	}
	else {
		listeEvents->push_back(*currentEvent);
	}
	// causing bug
	//delete currentEvent
	close();
}

void eventEdit::on_rdbManuel_toggled(bool checked)
{
	if (checked) {
		delete currentEvent->declencheur();
		currentEvent->setDeclencheur(new CManualDeclencheur());
	}
}

void eventEdit::on_rdbTime_toggled(bool checked)
{
	// TODO
	if (checked) {
		CTimeDeclencheur *declencheur = new CTimeDeclencheur();
		declencheur->setTime(qte_Temps->time().toString());
		currentEvent->setDeclencheur(declencheur);
		//currentEvent->declencheur()->
	}
}

void eventEdit::on_qle_Nom_textChanged(const QString &arg1)
{
	currentEvent->setName(arg1);
}

void eventEdit::on_pte_Description_textChanged()
{
	currentEvent->setDescription(pte_Description->toPlainText());
}

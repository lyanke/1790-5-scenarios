#ifndef CACTION_H
#define CACTION_H

#include <QString>
#include "object3d.h"

class CAction
{
	private:
		QString m_type ;
		Object3D m_object ;

	public:
		CAction(QString type, Object3D object);
		QString parametre() ;
		Object3D &object() ;
		QString type() {return m_type;}
		void setType(QString type) {m_type = type;}
		CAction(CAction& copy);
};

#endif // CACTION_H


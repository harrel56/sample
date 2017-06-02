#include "eventitem.h"

EventItem::EventItem(QListWidget * parent) : QListWidgetItem(parent) {}

EventItem::EventItem(int i, QDate dt, QString t, QString d) : id(i), date(dt), title(t), desc(d) {}


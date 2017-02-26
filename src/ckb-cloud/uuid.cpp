#include "uuid.h"

QString uuidToString(const QUuid &uuid)
{
    return uuid.toString().mid(1, 36);
}

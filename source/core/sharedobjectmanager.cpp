#include "sharedobjectmanager.h"


namespace Qs { 
void SharedObjectManager::close(const QByteArray& path) {
    QMutexLocker locker(&mutex);
    Ref& ref = pathToRef[path];
    munlock(ref.head, ref.allocated);
    munmap(ref.head, ref.allocated);
    pathToRef.remove(path);
}

void SharedObjectManager::closeAll() {
    QMutexLocker locker(&mutex);
    for (auto& ref : pathToRef) {
        munlock(ref.head, ref.allocated);
        munmap(ref.head, ref.allocated);
    }
    pathToRef.clear();
}

}

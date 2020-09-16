#ifndef KERNEL_SERVICE_H
#define KERNEL_SERVICE_H

#include <memory>

#include <targcontrol_repository.h>

class KernelService: public QObject {
    Q_OBJECT
public:
    KernelService(TargControlRepository* targ_repository);
    ~KernelService();

public slots:
    void do_work();
    void on_stop();

private:
    TargControlRepository* _targ_repository;
};

#endif // KERNEL_SERVICE_H

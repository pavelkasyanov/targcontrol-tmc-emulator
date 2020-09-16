#include "kernel_service.h"

KernelService::KernelService(TargControlRepository* targ_repository):
    _targ_repository(targ_repository)
{
}

KernelService::~KernelService()
{

}

void KernelService::do_work()
{
    _targ_repository->open_web_socket();
}

void KernelService::on_stop()
{

}

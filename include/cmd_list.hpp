#pragma once
#include "cmd_common.hpp"

int upload_file(int connfd, cmd_args & args);
int touch_save(int connfd, cmd_args & args);
int delete_file(int connfd, cmd_args & args);
int mount_save(int connfd, cmd_args & args);
int download_file(int connfd, cmd_args & args);
int delete_save(int connfd, cmd_args & args);
int unmount_save(int connfd, cmd_args & args);

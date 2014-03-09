
#ifndef _NGX_TCP_CMDSO_H_
#define _NGX_TCP_CMDSO_H_

#include <sys/types.h>
#include <stdint.h>
#include <netinet/in.h>

typedef struct ngx_tcp_ctx_s ngx_tcp_ctx_t;

typedef long (*ngx_tcp_send_data_pt)(ngx_tcp_ctx_t *ctx, 
                                     const u_char *data, 
                                     int len);

struct ngx_tcp_ctx_s {
    /* cmdso_sessioin array. the slot is init in cmdso_load func */
    void                  **cmdso_sessioin;
    void                   *ngx_tcp_session;
    ngx_tcp_send_data_pt    send_data;
};

typedef long (*cmd_pkg_handler_pt)(ngx_tcp_ctx_t *ctx, 
                                   const u_char *pkg, 
                                   int pkg_len);

typedef long
(*cmd_pkg_handler_add_pt)(void *cycle_param, 
                          uint32_t cmd_min, uint32_t cmd_max,
                          cmd_pkg_handler_pt h);

#define CMDSO_LOAD          "cmdso_load"
#define CMDSO_UNLOAD        "cmdso_unload"
#define CMDSO_SESS_INIT     "cmdso_sess_init"
#define CMDSO_SESS_FINIT    "cmdso_sess_finit"

typedef long 
(*cmdso_load_pt)(void *cycle_param, cmd_pkg_handler_add_pt add_h, int slot);
typedef long (*cmdso_unload_pt)(void *cycle_param);
typedef long (*cmdso_sess_init_pt)(ngx_tcp_ctx_t *ctx);
typedef long (*cmdso_sess_finit_pt)(ngx_tcp_ctx_t *ctx);

typedef struct {
    void                *handle;
    cmdso_load_pt        cmdso_load;
    cmdso_unload_pt      cmdso_unload;
    cmdso_sess_init_pt   cmdso_sess_init;
    cmdso_sess_finit_pt  cmdso_sess_finit;
} ngx_tcp_cmdso_t;


#pragma pack(push, 1)
typedef struct {
    /* size == pkg_head + pkg_body */
    uint32_t size;
    uint32_t cmd;

    /* padding */
    uint32_t spare0;
    uint32_t spare1;
    uint32_t spare2;
    uint32_t spare3;
    uint32_t spare4;
    uint32_t spare5;
} ngx_tcp_cmd_pkghead_t;
typedef struct {
    pid_t      dest_pid;
    int32_t    dest_fd;
    uint32_t   data_size;
    u_char     data[0];
} ngx_tcp_cmd_pkgtran_t;
#define CMD_SESSION_PKG_HEAD_LEN sizeof(ngx_tcp_cmd_pkghead_t)
#pragma pack(pop)


static inline void 
ngx_tcp_cmd_pkghead_hton(ngx_tcp_cmd_pkghead_t *pkghead)
{
    pkghead->size      = htonl(pkghead->size);
    pkghead->cmd       = htonl(pkghead->cmd);
}


static inline void 
ngx_tcp_cmd_pkghead_ntoh(ngx_tcp_cmd_pkghead_t *pkghead)
{
    pkghead->size      = ntohl(pkghead->size);
    pkghead->cmd       = ntohl(pkghead->cmd);
}


#define NGX_TCP_CMD_KEEPALIVE 1
#define NGX_TCP_CMD_TRAN 2
#define NGX_TCP_CMD_MAX_PKG_SIZE (1024 * 1024 * 4)

#endif


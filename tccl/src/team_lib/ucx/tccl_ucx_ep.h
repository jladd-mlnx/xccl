/*
 * Copyright (C) Mellanox Technologies Ltd. 2001-2020.  ALL RIGHTS RESERVED.
 * See file LICENSE for terms.
 */
#ifndef TCCL_UCX_EP_H_
#define TCCL_UCX_EP_H_
#include "tccl_ucx_context.h"
#include "tccl_ucx_team.h"
#include <ucp/api/ucp.h>

static inline tccl_status_t close_eps(ucp_ep_h *eps, int n_eps, ucp_worker_h worker)
{
    void *close_req;
    ucs_status_t status;
    int i;
    for(i = 0; i < n_eps; i++){
        if (!eps[i]) {
            continue;
        }
        close_req = ucp_ep_close_nb(eps[i], UCP_EP_CLOSE_MODE_FLUSH);
        if (UCS_PTR_IS_ERR(close_req)) {
            fprintf(stderr, "failed to start ep close, ep %p", eps[i]);
        }
        status = UCS_PTR_STATUS(close_req);
        if (status != UCS_OK) {
            while (status != UCS_OK) {
                ucp_worker_progress(worker);
                status = ucp_request_check_status(close_req);
            }
            ucp_request_free(close_req);
        }
    }
    return status;
}

static inline tccl_status_t
connect_ep(tccl_team_lib_ucx_context_t *ctx, tccl_ucx_team_t *team,
           tccl_team_config_t *cfg, char *addr_array, size_t max_addrlen, int rank)
{
    ucp_address_t *address = (ucp_address_t*)(addr_array + max_addrlen*rank);
    ucp_ep_params_t ep_params;
    ucs_status_t status;
    ucp_ep_h *ep;
    if (ctx->ucp_eps) {
        ep = &ctx->ucp_eps[tccl_team_rank_to_world(cfg, rank)];
    } else {
        ep = &team->ucp_eps[rank];
    }
    if (*ep) {
        return TCCL_OK;
    }
    ep_params.field_mask = UCP_EP_PARAM_FIELD_REMOTE_ADDRESS;
    ep_params.address    = address;

    status = ucp_ep_create(ctx->ucp_worker, &ep_params, ep);

    if (UCS_OK != status) {
        fprintf(stderr, "UCX returned connect error: %s\n",
                ucs_status_string(status));
        return TCCL_ERR_UNREACHABLE;
    }
    return TCCL_OK;
}

#endif

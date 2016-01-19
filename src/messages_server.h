/**
 * \file messages_server.h
 * \author Michal Vasko <mvasko@cesnet.cz>
 * \brief libnetconf2's functions and structures of server NETCONF messages.
 *
 * Copyright (c) 2015 CESNET, z.s.p.o.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of the Company nor the names of its contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 */

#ifndef NC_MESSAGES_SERVER_H_
#define NC_MESSAGES_SERVER_H_

#include <stdint.h>

#include "netconf.h"

typedef enum NC_ERROR {
    NC_ERR_UNKNOWN = 0,
    NC_ERR_IN_USE,
    NC_ERR_INVALID_VALUE,
    NC_ERR_TOO_BIG,
    NC_ERR_MISSING_ATTR,
    NC_ERR_BAD_ATTR,
    NC_ERR_UNKNOWN_ATTR,
    NC_ERR_MISSING_ELEM,
    NC_ERR_BAD_ELEM,
    NC_ERR_UNKNOWN_ELEM,
    NC_ERR_UNKNOWN_NS,
    NC_ERR_ACCESS_DENIED,
    NC_ERR_LOCK_DENIED,
    NC_ERR_RES_DENIED,
    NC_ERR_ROLLBACK_FAILED,
    NC_ERR_DATA_EXISTS,
    NC_ERR_DATA_MISSING,
    NC_ERR_OP_NOT_SUPPORTED,
    NC_ERR_OP_FAILED,
    NC_ERR_MALFORMED_MSG
} NC_ERR;

typedef enum NC_ERROR_TYPE {
    NC_ERR_TYPE_UNKNOWN = 0,
    NC_ERR_TYPE_TRAN,
    NC_ERR_TYPE_RPC,
    NC_ERR_TYPE_PROT,
    NC_ERR_TYPE_APP
} NC_ERR_TYPE;

/**
 * @brief NETCONF server rpc-reply object
 */
struct nc_server_reply;

/**
 * @brief NETCONF server error structure
 */
struct nc_server_error;

/**
 * @brief Create an OK rpc-reply object.
 *
 * @return rpc-reply object, NULL on error.
 */
struct nc_server_reply *nc_server_reply_ok(void);

/**
 * @brief Create a DATA rpc-reply object.
 *
 * @param[in] data Tree with the data.
 * @param[in] paramtype Determines how the \p data parameter is treated.
 * @return rpc-reply object, NULL on error.
 */
struct nc_server_reply *nc_server_reply_data(struct lyd_node *data, NC_PARAMTYPE paramtype);

/**
 * @brief Create an ERROR rpc-reply object.
 *
 * @param[in] err Structure holding the error information. It will be freed with the returned object.
 * @return rpc-reply object, NULL on error.
 */
struct nc_server_reply *nc_server_reply_err(struct nc_server_error *err);

/**
 * @brief Add another error to an ERROR rpc-reply object. It will be freed with the returned object.
 *
 * @param[in] reply ERROR reply to add to.
 * @param[in] err Structure holding the additional error information.
 * @return 0 on success, -1 on errror.
 */
int nc_server_reply_add_err(struct nc_server_reply *reply, struct nc_server_error *err);

/**
 * @brief Create a server error structure. Its \<error-message\> is filled with
 * a general description of the specific error.
 *
 * @param[in] tag \<error-tag\> of the server error. According to the tag, the
 * specific additional parameters are required:
 * - #NC_ERR_IN_USE
 * - #NC_ERR_INVALID_VALUE
 * - #NC_ERR_ACCESS_DENIED
 * - #NC_ERR_ROLLBACK_FAILED
 * - #NC_ERR_NOT_SUPPORTED
 * - #NC_ERR_TOO_BIG
 * - #NC_ERR_RES_DENIED
 * - #NC_ERR_OP_FAILED
 *   - `NC_ERR_TYPE type;` - type (layer) of the error.
 * - #NC_ERR_MISSING_ATTR
 * - #NC_ERR_BAD_ATTR
 * - #NC_ERR_UNKNOWN_ATTR
 *   - `NC_ERR_TYPE type;` - type (layer) of the error.
 *   - `const char *attr_name;` - error \<bad-attribute\> value.
 *   - `const char *elem_name;` - error \<bad-element\> value.
 * - #NC_ERR_MISSING_ELEM
 * - #NC_ERR_BAD_ELEM
 * - #NC_ERR_UNKNOWN_ELEM
 *   - `NC_ERR_TYPE type;` - type (layer) of the error.
 *   - `const char *elem_name;` - error \<bad-element\> value.
 * - #NC_ERR_UNKNOWN_NS
 *   - `NC_ERR_TYPE type;` - type (layer) of the error.
 *   - `const char *elem_name;` - error \<bad-element\> value.
 *   - `const char *nc_name;` - error \<bad-namespace\> value.
 * - #NC_ERR_LOCK_DENIED
 *   - `uint32_t session_id;` - error \<session-id\> value.
 * - #NC_ERR_DATA_EXISTS
 * - #NC_ERR_DATA_MISSING
 * - #NC_ERR_MALFORMED_MESSAGE
 *   - no additional arguments
 * @return Server error structure, NULL on error.
 */
struct nc_server_error *nc_err(NC_ERR tag, ...);

/**
 * @brief Set the \<error-app-tag\> element of an error. Any previous value will be overwritten.
 *
 * @param[in] err Error to modify.
 * @param[in] error_app_tag New value of \<error-app-tag\>.
 * @return 0 on success, -1 on error.
 */
int nc_err_set_app_tag(struct nc_server_error *err, const char *error_app_tag);

/**
 * @brief Set the \<error-path\> element of an error. Any previous value will be overwritten.
 *
 * @param[in] err Error to modify.
 * @param[in] error_path New value of \<error-path\>.
 * @return 0 on success, -1 on error.
 */
int nc_err_set_path(struct nc_server_error *err, const char *error_path);

/**
 * @brief Set the \<error-message\> element of an error. Any previous value will be overwritten.
 *
 * @param[in] err Error to modify.
 * @param[in] error_message New value of \<error-message\>.
 * @param[in] lang Optional language of \p error-message.
 * @return 0 on success, -1 on error.
 */
int nc_err_set_msg(struct nc_server_error *err, const char *error_message, const char *lang);

/**
 * @brief Set the \<session-id\> element of an error. Any previous value will be overwritten.
 *
 * @param[in] err Error to modify.
 * @param[in] session-id New value of \<session-id\>.
 * @return 0 on success, -1 on error.
 */
int nc_err_set_sid(struct nc_server_error *err, uint32_t session_id);

/**
 * @brief Add a \<bad-attribute\> element to an error.
 *
 * @param[in] err Error to modify.
 * @param[in] attr_name Value of the new \<bad-attribute\> element.
 * @return 0 on success, -1 on error.
 */
int nc_err_add_bad_attr(struct nc_server_error *err, const char *attr_name);

/**
 * @brief Add a \<bad-element\> element to an error.
 *
 * @param[in] err Error to modify.
 * @param[in] elem_name Value of the new \<bad-element\> element.
 * @return 0 on success, -1 on error.
 */
int nc_err_add_bad_elem(struct nc_server_error *err, const char *elem_name);

/**
 * @brief Add a \<bad-namespace\> element to an error.
 *
 * @param[in] err Error to modify.
 * @param[in] ns_name Value of the new \<bad-namespace\> element.
 * @return 0 on success, -1 on error.
 */
int nc_err_add_bad_ns(struct nc_server_error *err, const char *ns_name);

/**
 * @brief Add an additional custom element to an error.
 *
 * @param[in] err Error to modify.
 * @param[in] other New custom XML element.
 * @return 0 on success, -1 on error.
 */
int nc_err_add_info_other(struct nc_server_error *err, struct lyxml_elem *other);

/**
 * @brief Free a server rpc-reply object.
 *
 * @param[in] reply Server rpc-reply object to free.
 */
void nc_server_reply_free(struct nc_server_reply *reply);

/**
 * @brief Free a server error structure.
 *
 * @param[in] err Error structure to free.
 */
void nc_err_free(struct nc_server_error *err);

#endif /* NC_MESSAGES_SERVER_H_ */
/*
 * $Source$
 * $Author$
 *
 * Copyright 1989,1990 by the Massachusetts Institute of Technology.
 *
 * For copying and distribution information, please see the file
 * <krb5/copyright.h>.
 *
 * Glue between Kerberos version and ISODE 6.0 version of structures.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_kekrp2ekrp_c[] =
"$Id$";
#endif	/* lint || saber */

#include <krb5/copyright.h>
#include <krb5/krb5.h>

/*#include <time.h> */
#include <isode/psap.h>
#include "KRB5-types.h"
#include "asn1glue.h"
#include "asn1defs.h"

#include <krb5/ext-proto.h>

/* ISODE defines max(a,b) */

struct type_KRB5_EncKDCRepPart *
krb5_enc_kdc_rep_part2KRB5_EncKDCRepPart(val, error)
const register krb5_enc_kdc_rep_part *val;
register int *error;
{
    register struct type_KRB5_EncKDCRepPart *retval;

    retval = (struct type_KRB5_EncKDCRepPart *)xmalloc(sizeof(*retval));
    if (!retval) {
	*error = ENOMEM;
	return(0);
    }
    xbzero(retval, sizeof(*retval));

    retval->confounder = val->confounder;
    retval->key = krb5_keyblock2KRB5_EncryptionKey(val->session, error);
    if (!retval->key) {
	xfree(retval);
	return(0);
    }
    retval->last__req = krb5_last_req2KRB5_LastReq(val->last_req, error);
    if (!retval->last__req) {
    errout:
	free_KRB5_EncKDCRepPart(retval);
	return(0);
    }
    retval->ctime = unix2gentime(val->ctime, error);
    if (!retval->ctime) {
	goto errout;
    }
    retval->key__exp = unix2gentime(val->key_exp, error);
    if (!retval->key__exp) {
	goto errout;
    }
    retval->flags = krb5_flags2KRB5_TicketFlags(val->flags, error);
    if (!retval->flags) {
	goto errout;
    }
    retval->authtime = unix2gentime(val->times.authtime, error);
    if (!retval->authtime) {
	goto errout;
    }
    retval->starttime = unix2gentime(val->times.starttime, error);
    if (!retval->starttime) {
	goto errout;
    }
    retval->endtime = unix2gentime(val->times.endtime, error);
    if (!retval->endtime) {
	goto errout;
    }
    if (val->flags & TKT_FLG_RENEWABLE) {
	retval->renew__till = unix2gentime(val->times.renew_till, error);
	if (!retval->renew__till) {
	    goto errout;
	}
    }
    retval->srealm = krb5_data2qbuf(val->server[0]);
    if (!retval->srealm) {
	*error = ENOMEM;
	goto errout;
    }
    retval->sname = krb5_principal2KRB5_PrincipalName(val->server, error);
    if (!retval->sname) {
	goto errout;
    }
    retval->caddr = krb5_address2KRB5_HostAddresses(val->caddrs, error);
    if (!retval->caddr) {
	goto errout;
    }
    return(retval);
}

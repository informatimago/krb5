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
static char rcsid_kdcr2kkdcr_c[] =
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

krb5_kdc_rep *
KRB5_KDC__REP2krb5_kdc_rep(val, type, error)
const register struct type_KRB5_KDC__REP *val;
krb5_msgtype *type;
register int *error;
{
    register krb5_kdc_rep *retval;
    krb5_data *temp;

    retval = (krb5_kdc_rep *)xmalloc(sizeof(*retval));
    if (!retval) {
	*error = ENOMEM;
	return(0);
    }
    xbzero(retval, sizeof(*retval));

    *type = val->msg__type;

    retval->client = KRB5_PrincipalName2krb5_principal(val->cname,
						       val->crealm,
						       error);
    if (!retval->client) {
	xfree(retval);
	return(0);
    }

    retval->etype = val->etype;
    retval->ckvno = val->ckvno;

    retval->ticket = KRB5_Ticket2krb5_ticket(val->ticket, error);
    if (!retval->ticket) {
	krb5_free_kdc_rep(retval);
	return(0);
    }
    temp = qbuf2krb5_data(val->enc__part, error);
    if (temp) {
	retval->enc_part = *temp;
	xfree(temp);
    } else {
	krb5_free_kdc_rep(retval);
	return(0);
    }
    return(retval);
}

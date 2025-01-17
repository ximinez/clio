#ifndef REPORTING_HANDLERS_H_INCLUDED
#define REPORTING_HANDLERS_H_INCLUDED

#include <rpc/RPC.h>

namespace RPC
{
    /*
     * This file just contains declarations for all of the handlers
     */

// account state methods
Result  
doAccountInfo(Context const& context);

Result
doAccountChannels(Context const& context);

Result
doAccountCurrencies(Context const& context);

Result
doAccountLines(Context const& context);

Result  
doAccountObjects(Context const& context);

Result
doAccountOffers(Context const& context);

// channels methods

Result  
doChannelAuthorize(Context const& context);

Result
doChannelVerify(Context const& context);

// offers methods
Result
doBookOffers(Context const& context);

// ledger methods
Result  
doLedger(Context const& context);

Result
doLedgerEntry(Context const& context);

Result
doLedgerData(Context const& context);

Result
doLedgerRange(Context const& context);

// transaction methods
Result  
doTx(Context const& context);

Result
doAccountTx(Context const& context);

// subscriptions
Result  
doSubscribe(Context const& context);

Result
doUnsubscribe(Context const& context);

} // namespace RPC
#endif

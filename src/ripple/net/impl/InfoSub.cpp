

#include <ripple/net/InfoSub.h>
#include <atomic>

namespace ripple {




InfoSub::Source::Source (char const* name, Stoppable& parent)
    : Stoppable (name, parent)
{
}


InfoSub::InfoSub(Source& source)
    : m_source(source)
    , mSeq(assign_id())
{
}

InfoSub::InfoSub(Source& source, Consumer consumer)
    : m_consumer(consumer)
    , m_source(source)
    , mSeq(assign_id())
{
}

InfoSub::~InfoSub ()
{
    m_source.unsubTransactions (mSeq);
    m_source.unsubRTTransactions (mSeq);
    m_source.unsubLedger (mSeq);
    m_source.unsubManifests (mSeq);
    m_source.unsubServer (mSeq);
    m_source.unsubValidations (mSeq);
    m_source.unsubPeerStatus (mSeq);

    if (! realTimeSubscriptions_.empty ())
        m_source.unsubAccountInternal
            (mSeq, realTimeSubscriptions_, true);

    if (! normalSubscriptions_.empty ())
        m_source.unsubAccountInternal
            (mSeq, normalSubscriptions_, false);
}

Resource::Consumer& InfoSub::getConsumer()
{
    return m_consumer;
}

std::uint64_t InfoSub::getSeq ()
{
    return mSeq;
}

void InfoSub::onSendEmpty ()
{
}

void InfoSub::insertSubAccountInfo (AccountID const& account, bool rt)
{
    ScopedLockType sl (mLock);

    if (rt)
        realTimeSubscriptions_.insert (account);
    else
        normalSubscriptions_.insert (account);
}

void InfoSub::deleteSubAccountInfo (AccountID const& account, bool rt)
{
    ScopedLockType sl (mLock);

    if (rt)
        realTimeSubscriptions_.erase (account);
    else
        normalSubscriptions_.erase (account);
}

void InfoSub::clearPathRequest ()
{
    mPathRequest.reset ();
}

void InfoSub::setPathRequest (const std::shared_ptr<PathRequest>& req)
{
    mPathRequest = req;
}

const std::shared_ptr<PathRequest>& InfoSub::getPathRequest ()
{
    return mPathRequest;
}

} 

























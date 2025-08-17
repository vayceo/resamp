#include "EventHandler.h"
#include "Event.h"
#include "EventHandlerHistory.h"

void CEventHandlerHistory::InjectHooks()
{

}

CEventHandlerHistory::~CEventHandlerHistory()
{
    Flush();
}

// 0x4BC550
void CEventHandlerHistory::ClearAllEvents()
{
    ClearTempEvent();
    ClearNonTempEvent();
}

// 0x4B8C60
void CEventHandlerHistory::ClearNonTempEvent()
{
    delete m_nonTempEvent;
    m_nonTempEvent = nullptr;
}

// 0x4B8C40
void CEventHandlerHistory::ClearTempEvent()
{
    delete m_tempEvent;
    m_tempEvent = nullptr;
}

void CEventHandlerHistory::ClearStoredActiveEvent()
{
    delete m_storedActiveEvent;
    m_storedActiveEvent = nullptr;
}

void CEventHandlerHistory::Flush()
{
    ClearAllEvents();
    ClearStoredActiveEvent();
}

// 0x4B8C80
int32 CEventHandlerHistory::GetCurrentEventPriority()
{
    if (m_tempEvent)
        return m_tempEvent->GetEventPriority();
    if (m_nonTempEvent)
        return m_nonTempEvent->GetEventPriority();
    return -1;
}

// 0x4B8B90
bool CEventHandlerHistory::IsRespondingToEvent(eEventType eventType)
{
    if (eventType == EVENT_NONE)
        return m_nonTempEvent || m_storedActiveEvent;

    bool isNonTempEvent = false;
    if (m_nonTempEvent && m_nonTempEvent->GetEventType() == eventType)
        isNonTempEvent = true;

    if (!m_storedActiveEvent || m_storedActiveEvent->GetEventType() != eventType)
        return isNonTempEvent;

    return true;
}

// 0x4BC4B0
void CEventHandlerHistory::RecordCurrentEvent(CPedGTA* ped, CEvent& event)
{
    if (event.GetEventType() != EVENT_SCRIPT_COMMAND) {
        if (CEventHandler::IsTemporaryEvent(event)) {
            if (m_nonTempEvent) {
                StoreActiveEvent();
                m_nonTempEvent = nullptr;
            }
            ClearTempEvent();
            m_tempEvent = event.Clone();
        }
        else {
            ClearNonTempEvent();
            m_nonTempEvent = event.Clone();
            ClearStoredActiveEvent();
            ClearTempEvent();
        }
    }
}

// 0x4B8BF0
void CEventHandlerHistory::StoreActiveEvent()
{
    delete m_storedActiveEvent;
    m_storedActiveEvent = m_nonTempEvent;
    m_storedActiveEventTimer.Start(2000);
}

// 0x4BC580
bool CEventHandlerHistory::TakesPriorityOverCurrentEvent(CEvent& event)
{
    if (m_nonTempEvent)
        return event.TakesPriorityOver(*m_nonTempEvent);
    if (!m_tempEvent)
        return true;
    if (CEventHandler::IsTemporaryEvent(event))
        return event.TakesPriorityOver(*m_tempEvent);
    if (!event.TakesPriorityOver(*m_tempEvent))
        return false;
    if (!m_storedActiveEvent || event.TakesPriorityOver(*m_storedActiveEvent))
        return true;
    return false;
}

// 0x4B8C20
void CEventHandlerHistory::TickStoredEvent(CPedGTA* ped)
{
    if (m_storedActiveEvent && !m_tempEvent) {
        m_nonTempEvent = m_storedActiveEvent;
        m_storedActiveEvent = nullptr;
    }
}


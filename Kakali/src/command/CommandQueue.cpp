#include "CommandQueue.h"

bool CommandQueue::PushBack(Command&& command)
{
    if (m_back == COMMAND_QUEUE_SIZE) {
        ShiftFront();
    }

    if (m_back != m_selector + 1) {
        m_back = m_selector + 1;
    }
    m_data[m_back++] = std::move(command);
    m_data[++m_selector].Execute();
    NG_CORE_LOG_INFO("Push... Selector: {}, Back: {}", m_selector, m_back);
    return true;
}

void CommandQueue::RevertBack()
{
    if (m_back == 0 || m_selector < 0) {
        return;
    }
    NG_CORE_LOG_INFO("Undo... Selector: {}, Back: {}", m_selector, m_back);
    m_data[m_selector--].Revert();
}

void CommandQueue::RedoBack()
{
    if (m_selector + 1 == m_back) {
        return;
    }
    NG_CORE_LOG_INFO("Redo... Selector: {}, Back: {}", m_selector, m_back);
    m_data[++m_selector].Execute();
}

void CommandQueue::ShiftFront()
{
    if (m_back <= 1) {
        return;
    }

    for (size_t i = 0; i < m_back - 1; i++) {
        m_data[i] = std::move(m_data[i + 1]);
    }
    m_back--;
    m_selector--;
}

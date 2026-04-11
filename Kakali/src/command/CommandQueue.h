#include "Command.h"

class CommandQueue {
public:
    CommandQueue() = default;
    CommandQueue(const CommandQueue& other) = delete;
    CommandQueue(CommandQueue&& other) = delete;
    ~CommandQueue() = default;

    bool PushBack(Command&& command);

    template<typename T>
    bool PushBack(Command&& command, const T& data)
    {
        if (m_back == COMMAND_QUEUE_SIZE) {
            ShiftFront();
        }

        if (m_back != m_selector + 1) {
            m_back = m_selector + 1;
        }
        m_data[m_back++] = std::move(command);
        m_data[++m_selector].SetData<T>(data);
        m_data[m_selector].Execute();
        NG_CORE_LOG_INFO("Push... Selector: {}, Back: {}", m_selector, m_back);
        return true;
    }

    void RevertBack();
    void RedoBack();

    inline bool IsEmpty() const { return m_back == 0; }
    inline size_t Count() const { return m_back; }
private:
    void ShiftFront();
private:
    static constexpr size_t COMMAND_QUEUE_SIZE = 50;
    std::array<Command, COMMAND_QUEUE_SIZE> m_data;
    int64_t m_selector = -1;
    size_t m_back = 0;


};
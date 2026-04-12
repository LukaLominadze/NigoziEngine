#include <Nigozi.h>
#include <new>

class Command 
{
public:
	Command() = default;
	explicit Command(const std::function<void(void*)>& execute, const std::function<void(void*)>& revert)
		:m_execute(execute), m_revert(revert) {}
	~Command() = default;

	template<typename T>
	inline void SetData(const T& data) {
		static_assert(sizeof(T) <= COMMAND_DATA_SIZE, "[Command] Data size must be lower or equal to buffer size!");
		new (p_data) T(data);
	}

	inline void Execute() { m_execute(((void*)p_data)); }
	inline void Revert() { m_revert((void*)p_data); }
private:
	std::function<void(void*)> m_execute;
	std::function<void(void*)> m_revert;

	static constexpr size_t COMMAND_DATA_SIZE = 96;
	char p_data[COMMAND_DATA_SIZE];
};
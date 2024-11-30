#include <iostream>
#include <utility> 

// Пространство имен для управления уникальными указателями
namespace UniquePointerNamespace {

    // Перечисление для состояния указателя
    enum class PointerState {
        EMPTY,   // Указатель не указывает ни на что
        VALID,   // Указатель указывает на действительный объект
        RELEASED // Указатель указывает на освобожденный объект
    };

    template<typename T>
    class UniqPtr
    {
    private:
        T* ptr;
        PointerState state; // Состояние указателя

    public:
        // Конструктор по умолчанию
        UniqPtr() : ptr(nullptr), state(PointerState::EMPTY) {}

        // Конструктор с указателем
        explicit UniqPtr(T* p) : ptr(p), state(PointerState::VALID) {}

        // Запрет копирования
        UniqPtr(const UniqPtr&) = delete;
        UniqPtr& operator=(const UniqPtr&) = delete;

        // Перемещение
        UniqPtr(UniqPtr&& other) noexcept : ptr(other.ptr), state(other.state)
        {
            other.ptr = nullptr;
            other.state = PointerState::EMPTY; // Обновление состояния
        }

        UniqPtr& operator=(UniqPtr&& other) noexcept
        {
            if (this != &other)
            {
                delete ptr;
                ptr = other.ptr;
                state = other.state; //Обновление состояния
                other.ptr = nullptr;
                other.state = PointerState::EMPTY; // Обновление состояния
            }
            return *this;
        }

        // Деструктор
        ~UniqPtr()
        {
            delete ptr;
        }

        // Метод swap
        void swap(UniqPtr& other)
        {
            std::swap(ptr, other.ptr);
            std::swap(state, other.state); // Меняем местами состояния
        }

        // Метод free
        void free()
        {
            delete ptr;
            ptr = nullptr;
            state = PointerState::RELEASED; // Обновление состояния
        }

        // Метод release
        T* release()
        {
            T* temp = ptr;
            ptr = nullptr;
            state = PointerState::RELEASED; // Обновление состояния
            return temp;
        }

        // Оператор разыменования
        T& operator*() const
        {
            return *ptr;
        }

        // Оператор доступа к члену
        T* operator->() const
        {
            return ptr;
        }

        // Проверка на наличие объекта
        explicit operator bool() const
        {
            return ptr != nullptr; // Проверяем, указывает ли указатель на объект
        }

        // Метод для получения состояния указателя
        PointerState getState() const {
            return state;
        }
    };
}

int main()
{
    using namespace UniquePointerNamespace; // Использование пространства имен

    UniqPtr<int> ptr1(new int(42));
    std::cout << *ptr1 << std::endl;

    UniqPtr<int> ptr2 = std::move(ptr1);
    std::cout << *ptr2 << std::endl;

    ptr2.free();

    // Проверка состояния указателя после освобождения
    if (ptr2.getState() == PointerState::RELEASED) {
        std::cout << "ptr2 has been released." << std::endl;
    }

    return 0;
}

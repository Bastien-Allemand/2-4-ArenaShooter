#ifndef ENGINE_TIMER_MANAGER_HPP_INCLUDED
#define ENGINE_TIMER_MANAGER_HPP_INCLUDED

#include "define.h"
#include <functional>
#include "GameObject.h"

namespace gce 
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // @brief A simple timer manager that allows scheduling functions to be executed after a delay.
	// @note This manager supports multiple timers, each with its own delay and associated function.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    class TimerBase {
    public:
        float remainingTime = 0.0f;
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// @brief Virtual function to execute the timer's associated function.
		// @note This function should be overridden in derived classes to define the specific action to take when the timer expires.
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void Execute() = 0;
        virtual ~TimerBase() = default;
    };

    template<typename... Args>
    class Timer : public TimerBase {
    public:
        using FuncPtr = void(*)(Args...);

        //////////////////////////////////////////////////////////////////////////////////////////////
        // @brief Constructor that takes a delay, a function pointer, and any number of arguments.
		// @param delay The time in seconds after which the function will be executed.
		// @param func The function to be executed after the delay.
		// @param args The arguments to be passed to the function when it is executed.
		//////////////////////////////////////////////////////////////////////////////////////////////
        Timer(float delay, FuncPtr func, Args... args)
            : m_func(func), m_args(std::make_tuple(std::forward<Args>(args)...)) {
            remainingTime = delay;
        }

		///////////////////////////////////////////////////////////////////////
		// @brief Executes the stored function with the provided arguments.
		// This method is called when the timer reaches zero.
		///////////////////////////////////////////////////////////////////////
        void Execute() override {
            std::apply(m_func, m_args);
        }

    private:
        FuncPtr m_func;
        std::tuple<Args...> m_args;
    };

    class TimerManager {
    public:
        ///////////////////////////////////////////////////
		// @brief Default constructor for TimerManager.
		///////////////////////////////////////////////////
        TimerManager() = default;
        //////////////////////////////////////////////////
		// @brief Default destructor for TimerManager.
		//////////////////////////////////////////////////
        ~TimerManager() = default;

       

        
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////S
        // @brief Adds a timer that will execute a function after a specified delay.
		// @param delay The time in seconds after which the function will be executed.
		// @param func The function to be executed after the delay.
		// @param args The arguments to be passed to the function when it is executed.
		// @note The function must be callable with the provided arguments.
		// @note The Args are perfectly forwarded to the Timer constructor, allowing for any type of arguments.
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////
        template <typename Func, typename... Args>
        void AddTimer(float delay, Func* func, Args&&... args) {
            using TimerT = Timer<std::decay_t<Args>...>;
            m_timers.emplace_back(
                std::make_unique<TimerT>(delay, func, std::forward<Args>(args)...)
            );
        }

		////////////////////////////////////////////////////////////////////////////////////////////////////
        // @brief Deletes all timers and clears the timer list.
		// @note This function is useful for resetting the timer manager, removing all scheduled timers.
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        void Clear() {
            m_timers.clear();
        }

		/////////////////////////////////////////////////////////////////////////////////////////////////////////
        // @brief Updates all timers, executing those that have reached their delay.
		// @param deltaTime The time elapsed since the last update, used to decrement the timers.
		// @note This function should be called regularly (e.g., every frame) to ensure timers are processed.
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
        void Update(float deltaTime) {
            for (auto it = m_timers.begin(); it != m_timers.end(); ) {
                TimerBase* timer = it->get();
                timer->remainingTime -= deltaTime;
                if (timer->remainingTime <= 0.0f) {
                    timer->Execute();
                    it = m_timers.erase(it);
                }
                else {
                    ++it;
                }
            }
        }

    private:
        std::vector<std::unique_ptr<TimerBase>> m_timers;
    };

};
#endif 
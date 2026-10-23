/**
 * @file cron-schedule.hpp
 * @brief Include file for the cron-schedule library.
 * @author ohto-ai
*/

#pragma once

#include <functional>
#include <memory>
#include <list>
#include <map>
#include "croncpp/croncpp.h"

namespace ohtoai{
    namespace cron_schedule{

        class cron_task;
        class cron_schedule;
        template <size_t WheelSize> class timing_wheel;
        class timer_task;
        class timer_task_entry;
        class timer_task_list;
        
        class cron_schedule {
        public: // interface
            /// @brief run tick to invoke callback function.
            void tick();

            /// @brief add cron task.
            /// @param task_name The taks name. e.g. "myTask"
            /// @param cron The cron schedule. e.g. "0 0 0 * * *"
            /// @param callback The callback function.
            /// @return The cron task.
            cron_task& add_task(const std::string& task_name, const cron::cronexpr& cron, std::function<void(cron_task& )> callback);

            /// @brief task count
            /// @return task count
            size_t size() const;
        private: // member variables
            // todo: add member variables here.
            std::map<std::string, std::shared_ptr<cron_task>> tasks_;

        };

        class cron_task {
        public: // interface
            /// @brief Constructor.
            cron_task(const std::string&, const cron::cronexpr&, std::function<void(cron_task& )> callback);

            /// @brief Get the task name.
            /// @return The task name.
            const std::string& task_name() const;

            /// @brief Get the cron schedule.
            /// @return The cron schedule.
            const cron::cronexpr& cron() const;

            /// @brief Get the next time of the cron schedule.
            /// @param base_time The base time.
            /// @return The next time of the cron schedule.
            std::time_t next_time(const std::time_t& base_time) const;
        private: // member variables
            std::string task_name_;
            cron::cronexpr cron_;
            std::function<void(cron_task& )> callback_;
        };
    
        template <size_t WheelSize>
        class timing_wheel {
        public:
            void tick();
        private:
            std::array<timer_task_list, WheelSize> task_lists_;
        };

        using hour_timing_wheel = timing_wheel<24>;
        using minute_timing_wheel = timing_wheel<60>;
        using second_timing_wheel = timing_wheel<60>;

        class timer_task {
        public:
            timer_task(const std::string& task_name, const std::time_t& time, std::function<void(timer_task& )> callback);
            const std::string& task_name() const;
            const std::time_t& time() const;
            void invoke();
        private:
            std::string task_name_;
            std::time_t time_;
            std::function<void(timer_task& )> callback_;
        };

        class timer_task_entry {
        public:
            timer_task_entry(const std::string& task_name, const std::time_t& time, std::function<void(timer_task& )> callback);
            const std::string& task_name() const;
            const std::time_t& time() const;
            void invoke();
        private:
            std::string task_name_;
            std::time_t time_;
            std::function<void(timer_task& )> callback_;
        };

        class timer_task_list {
        public:
            void add(const timer_task_entry& entry);
            void remove(const std::string& task_name);
            void invoke();
        private:
            std::list<timer_task_entry> entries_;
        };
    }
}

/******************************************************************************/
// ohtoai::cron_schedule::cron_schedule
/******************************************************************************/
void ohtoai::cron_schedule::cron_schedule::tick() {
    // todo: implement here.
}

ohtoai::cron_schedule::cron_task& ohtoai::cron_schedule::cron_schedule::add_task(const std::string& task_name, const cron::cronexpr& cron, std::function<void(cron_task& )> callback) {
    // todo: implement here. Add task to time_wheel.
    auto p =  tasks_.emplace(task_name, std::make_shared<cron_task>(task_name, cron, callback));
    return *(p.first->second);
}

size_t ohtoai::cron_schedule::cron_schedule::size() const {
    return tasks_.size();
}

/******************************************************************************/
// ohtoai::cron_schedule::cron_task
/******************************************************************************/
ohtoai::cron_schedule::cron_task::cron_task(const std::string& task_name, const cron::cronexpr& cron, std::function<void(cron_task& )> callback)
    : task_name_(task_name)
    , cron_(cron)
    , callback_(callback)
{
}

const std::string& ohtoai::cron_schedule::cron_task::task_name() const {
    return task_name_;
}

const cron::cronexpr& ohtoai::cron_schedule::cron_task::cron() const {
    return cron_;
}

std::time_t ohtoai::cron_schedule::cron_task::next_time(const std::time_t& base_time) const {
    return cron::cron_next(cron_, base_time);
}

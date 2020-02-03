//Created: 4/12/2018 10:08:34 AM

/**
 \file
 \brief Self managing software timers.
 \details

 \date Created 2018-12-04
 \author Teddy.Hut
 */

#pragma once

#include <stdlib.h>
#include <avr/io.h>

#include "utility.h"
#include <timerhardware.h>

namespace libmodule
{
    namespace time
    {
        //Timers are organized as follows
        //(this could be better - in particular, allowing multiple user provided implementations for each frequency)
        //
        //---TimerBase---
        // - TimerBase has a template with "TickFrequency_c".
        // - Since it inherits from InstanceList, all timers of -that particular frequency- are kept in a list
        // - Specializations of TimerBase are added for each different TickFrequency
        // - These specializations implement the timer on the hardware using static functions
        // - These static functions will take care of calling tick() for all instances of that frequency when needed
        //
        //---Timer---
        // - Timer allows timers of different tick_t to be counted as instances of the same frequency

        template <size_t tickFrequency_c = 1000, typename tick_t = uint16_t>
        class Timer : public TimerBase<tickFrequency_c>
        {
        public:
            //Returns whether the timer is finished or not
            inline operator bool() const;
            inline operator tick_t() const;
            inline Timer &operator=(tick_t const p0);

            //Starts the timer
            virtual void start();
            //Stops (pauses) the timer
            virtual void stop();
            //Resets (sets everything to default value) the timer
            void reset();

            volatile tick_t ticks = 0;
            volatile bool finished : 1;
            volatile bool running : 1;

        protected:
            //Called when 1 tick interval has elapsed
            virtual void tick() override;
        };

        template <size_t tickFrequency_c = 1000, typename tick_t = uint16_t>
        class Stopwatch : public Timer<tickFrequency_c, tick_t>
        {
        public:
            //TODO: Why does this work? (extension? Why isn't it the norm?)
            using Timer<tickFrequency_c, tick_t>::operator =;
            //using Timer<tickFrequency_c, tick_t>::operator tick_t const;
            void start() override;
        protected:
            void tick() override;
        };

        //---Implementation---

        //Initializes/starts the daemons for the timers of the given TickFrequencies
        template<size_t ...freqs>
        void start_timer_daemons()
        {
            int dummy[sizeof...(freqs)] [[gnu::unused]] = {(TimerBase<freqs>::start_daemon(), 0)...};
        }

        template <size_t tickFrequency_c, typename tick_t /*= uint16_t*/>
        Timer<tickFrequency_c, tick_t>::operator tick_t() const
        {
            return ticks;
        }

        template <size_t tickFrequency_c, typename tick_t /*= uint16_t*/>
        Timer<tickFrequency_c, tick_t>::operator bool() const
        {
            return finished;
        }

        template <size_t tickFrequency_c, typename tick_t /*= uint16_t*/>
        Timer<tickFrequency_c, tick_t> &Timer<tickFrequency_c, tick_t>::operator=(tick_t const p0)
        {
            ticks = p0;
            return *this;
        }

        template <size_t tickFrequency_c, typename tick_t /*= uint16_t*/>
        void Timer<tickFrequency_c, tick_t>::tick()
        {
            if(running && --ticks == 0) {
                running = false;
                finished = true;
            }
        }

        template <size_t tickFrequency_c, typename tick_t /*= uint16_t*/>
        void Timer<tickFrequency_c, tick_t>::start()
        {
            if(ticks == 0) {
                running = false;
                finished = true;
                return;
            }
            running = true;
            finished = false;
        }

        template <size_t tickFrequency_c, typename tick_t /*= uint16_t*/>
        void Timer<tickFrequency_c, tick_t>::stop()
        {
            running = false;
        }

        template <size_t tickFrequency_c, typename tick_t /*= uint16_t*/>
        void Timer<tickFrequency_c, tick_t>::reset()
        {
            ticks = 0;
            finished = false;
            running = false;
        }

        template <size_t tickFrequency_c /*= 1000*/, typename tick_t /*= uint16_t*/>
        void Stopwatch<tickFrequency_c, tick_t>::start()
        {
            //I think 'this' has to be used because of the following (quoting en.cppreference dependent name page)
            /*
              Members of the current instantiation may be both dependent and non-dependent.
              If the lookup of a member of current instantiation gives a different result
              between the point of instantiation and the point of definition, the lookup is ambiguous.
              Note however that when a member name is used, it is not automatically converted to a class member access expression,
              only explicit member access expressions indicate members of current instantiation.
            */
            //Also see en.cppreference non-static member functions.
            //Also: In class templates, this is a dependent expression, and explicit this-> may be used to force another expression to become dependent.
            //As far as I understand:
            // - Since running is dependent on the type of base class Timer, it is not automatically assumed to be part of the current instantiation,
            // (during the definition phase, here), and therefore isn't converted to a member access expression.
            // - By explicitly mentioning 'this' it is converted to a member access expression.
            this->running = true;
        }

        template <size_t tickFrequency_c /*= 1000*/, typename tick_t /*= uint16_t*/>
        void Stopwatch<tickFrequency_c, tick_t>::tick()
        {
            if(this->running)
                this->ticks++;
        }

    } //timer

//Type aliases
    using Timer1k = time::Timer<1000>;
    using Stopwatch1k = time::Stopwatch<1000>;

} //libmodule

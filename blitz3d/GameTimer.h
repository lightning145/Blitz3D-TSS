#ifndef GAMETIMER_H
#define GAMETIMER_H

#include <windows.h>

class GameTimer {
public:
    GameTimer() {
        Reset();
        m_firstCall = true;
    }

    void Reset() {
        QueryPerformanceFrequency(&m_frequency);
        QueryPerformanceCounter(&m_startTime);
        m_lastTime = 0.0;
        m_pausedTime = 0.0;
        m_paused = false;
        m_firstCall = true;
    }

    void Pause() {
        if (!m_paused) {
            m_pausedTime = getCurrentTime();
            m_paused = true;
        }
    }

    void Resume() {
        if (m_paused) {
            LARGE_INTEGER newStart;
            QueryPerformanceCounter(&newStart);
            m_startTime.QuadPart = newStart.QuadPart -
                static_cast<LONGLONG>((m_pausedTime * m_frequency.QuadPart));
            m_paused = false;
        }
    }

    double getCurrentTime() const {
        if (m_paused) return m_pausedTime;

        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime);
        return static_cast<double>(currentTime.QuadPart - m_startTime.QuadPart) /
            m_frequency.QuadPart;
    }

    float GetDeltaTime() {
        if (m_paused) {
            return 0.0f;
        }

        double currentTime = getCurrentTime();

        if (m_firstCall) {
            m_lastTime = currentTime;
            m_firstCall = false;
            return 0.0f;
        }

        float deltaTime = static_cast<float>(currentTime - m_lastTime);
        m_lastTime = currentTime;
        return deltaTime;
    }

    bool IsPaused() const { return m_paused; }

private:
    LARGE_INTEGER m_frequency;
    LARGE_INTEGER m_startTime;
    double m_lastTime;
    double m_pausedTime;
    bool m_paused;
    bool m_firstCall;
};

#endif
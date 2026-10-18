#pragma once
#include <string>
#include <map>
#include <functional>
#include <memory>

using StateFunction = std::function<void()>;

struct State
{
    StateFunction onEnter; 
    StateFunction onUpdate;
    StateFunction onExit;  

    State() = default;
    State(const StateFunction& enter, const StateFunction& update, const StateFunction& exit)
        : onEnter(enter), onUpdate(update), onExit(exit) {
    }
};

class StateMachine
{
private:
    std::map<std::string, State> m_States;

    std::string m_CurrentStateName;

public:
    StateMachine() : m_CurrentStateName("") {}

    void AddState(const std::string& stateName, const State& state)
    {
        m_States[stateName] = state;
    }

    void ChangeState(const std::string& newStateName)
    {
        if (m_CurrentStateName == newStateName)
        {
            return;
        }

        if (!m_CurrentStateName.empty() && m_States.find(m_CurrentStateName) != m_States.end())
        {
            if (m_States[m_CurrentStateName].onExit)
            {
                m_States[m_CurrentStateName].onExit();
            }
        }

        m_CurrentStateName = newStateName;

        if (m_States.find(m_CurrentStateName) != m_States.end())
        {
            if (m_States[m_CurrentStateName].onEnter)
            {
                m_States[m_CurrentStateName].onEnter();
            }
        }
    }

    void Update()
    {
        if (!m_CurrentStateName.empty() && m_States.find(m_CurrentStateName) != m_States.end())
        {
            if (m_States[m_CurrentStateName].onUpdate)
            {
                m_States[m_CurrentStateName].onUpdate();
            }
        }
    }

    std::string GetCurrentStateName() const
    {
        return m_CurrentStateName;
    }
};

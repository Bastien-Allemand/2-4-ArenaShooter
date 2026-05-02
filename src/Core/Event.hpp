#ifndef CORE_EVENT_HPP_INCLUDED
#define CORE_EVENT_HPP_INCLUDED

#include <Containers/Vector.hpp>
#include "define.h"

namespace gce {


template <typename ...Args>
struct Event
{
    struct ListenerBase;
    using function = void(*)(Args...);
    template <class C> using method = void(C::*)(Args...);
    template <class C> using methodConst = void(C::*)(Args...) const;

    Event() = default;
    ~Event() { ClearListeners(); }

    Event( Event const& ) = default;
    Event( Event&& ) = default;
    Event& operator=( Event const& ) = default;
    Event& operator=( Event&& ) = default;

    void Invoke( Args... args );

    ListenerBase* AddListener( function pListener );
    template <class C> ListenerBase* AddListener( method<C> pListener, C* pClass );
    template <class C> ListenerBase* AddListener( methodConst<C> pListener, C const* pClass );
    void RemoveListener( ListenerBase* pListener );
    void ClearListeners();
    
private:
    struct FunctionListener;
    template <class C> struct MethodListener;
    template <class C> struct MethodListenerConst;

    Vector<ListenerBase*> m_listeners;
};


template <typename ...Args>
struct Event<Args...>::ListenerBase
{
    ListenerBase() = default;
    virtual ~ListenerBase() = default;

    ListenerBase( ListenerBase const& ) = delete;
    ListenerBase( ListenerBase&& ) = delete;
    ListenerBase& operator=( ListenerBase const& ) = delete;
    ListenerBase& operator=( ListenerBase&& ) = delete;

    virtual void Invoke( Args... args ) = 0;
};


template <typename ...Args>
struct Event<Args...>::FunctionListener final : ListenerBase
{
    explicit FunctionListener( function const pFunction ): m_pFunction( pFunction ) {}
    ~FunctionListener() override = default;

    FunctionListener( FunctionListener const& ) = delete;
    FunctionListener( FunctionListener&& ) = delete;
    FunctionListener& operator=( FunctionListener const& ) = delete;
    FunctionListener& operator=( FunctionListener&& ) = delete;

    void Invoke( Args... args ) override { m_pFunction( args... ); }

private:
    function m_pFunction = nullptr;
};


template <typename ...Args>
template <class C>
struct Event<Args...>::MethodListener final : ListenerBase
{
    explicit MethodListener( method<C> const pMethod, C* const pClass ): m_pMethod( pMethod ), m_pClass( pClass ) {}
    ~MethodListener() override = default;

    MethodListener( MethodListener const& ) = delete;
    MethodListener( MethodListener&& ) = delete;
    MethodListener& operator=( MethodListener const& ) = delete;
    MethodListener& operator=( MethodListener&& ) = delete;

    void Invoke( Args... args ) override { (m_pClass->*m_pMethod)( args... ); }

private:
    method<C> m_pMethod = nullptr;
    C* m_pClass = nullptr;
};


template <typename ...Args>
template <class C>
struct Event<Args...>::MethodListenerConst final : ListenerBase
{
    explicit MethodListenerConst( methodConst<C> const pMethod, C const* const pClass ): m_pMethod( pMethod ), m_pClass( pClass ) {}
    ~MethodListenerConst() override = default;

    MethodListenerConst( MethodListenerConst const& ) = delete;
    MethodListenerConst( MethodListenerConst&& ) = delete;
    MethodListenerConst& operator=( MethodListenerConst const& ) = delete;
    MethodListenerConst& operator=( MethodListenerConst&& ) = delete;

    void Invoke( Args... args ) override { (m_pClass->*m_pMethod)( args... ); }

private:
    methodConst<C> m_pMethod = nullptr;
    C const* m_pClass = nullptr;
};


template <typename... Args>
void Event<Args...>::Invoke(Args... args) { for ( ListenerBase* const listener : m_listeners ) listener->Invoke( args... ); }


template <typename ...Args>
typename Event<Args...>::ListenerBase* Event<Args...>::AddListener( function const pListener )
{
    ListenerBase* const pResult = new FunctionListener( pListener );
    m_listeners.PushBack( pResult );
    return pResult;
}

template <typename ...Args> template <class C>
typename Event<Args...>::ListenerBase* Event<Args...>::AddListener( method<C> const pListener, C* const pClass )
{
    ListenerBase* const pResult = new MethodListener<C>( pListener, pClass );
    m_listeners.PushBack( pResult );
    return pResult;
}

template <typename ...Args> template <class C>
typename Event<Args...>::ListenerBase* Event<Args...>::AddListener( methodConst<C> const pListener, C const* const pClass )
{
    ListenerBase* const pResult = new MethodListenerConst<C>( pListener, pClass );
    m_listeners.PushBack( pResult );
    return pResult;
}

template <typename... Args>
void Event<Args...>::RemoveListener( ListenerBase* const pListener )
{
    for ( auto it = m_listeners.begin(); it != m_listeners.end(); ++it )
    {
        if ( *it != pListener ) continue;
        m_listeners.Erase( it );
        delete pListener;
        return;     
    }
}

template <typename... Args>
void Event<Args...>::ClearListeners()
{
    for ( ListenerBase const* const pListener : m_listeners ) delete pListener;
    m_listeners.Clear();
}


}

#endif
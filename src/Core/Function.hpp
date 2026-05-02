#ifndef FUNCTION_HPP_INCLUDED
#define FUNCTION_HPP_INCLUDED


// Base Template Struct
template <typename F> struct Function
{
    Function( F lambda ): m_lambda( lambda ) {}

    auto operator()( auto... args ) { return m_lambda( args... ); }

protected:
     F m_lambda;
};


// FunctionBase
template <typename R, typename ...Args>
struct Function<R(*const)(Args...)>
{
    using function = R(*)(Args...);
    
    Function() = default;
    virtual ~Function() = default;

    Function( Function const& ) = default;
    Function( Function&& ) = default;
    Function& operator=( Function const& ) = default;
    Function& operator=( Function&& ) = default;

    virtual R operator()( Args... args ) const = 0;
};



// Function
template <typename R, typename ...Args>
struct Function<R(*)(Args...)>: Function<R(*const)(Args...)>
{
    using function = R(*)(Args...);
    
    Function() = default;
    ~Function() override = default;

    Function( function const pFunction ): m_pFunction( pFunction ) {}
    Function& operator=( function const pFunction ) { m_pFunction = pFunction; return *this; }

    Function( Function const& ) = default;
    Function( Function&& ) = default;
    Function& operator=( Function const& ) = default;
    Function& operator=( Function&& ) = default;

    R operator()( Args... args ) const override { return m_pFunction( args... ); }

protected:
    function m_pFunction = nullptr;
};


// Method
template <typename C, typename R, typename ...Args>
struct Function<R(C::*)(Args...)>: Function<R(*const)(Args...)>
{
    using function = R(C::*)(Args...);

    Function() = default;
    ~Function() override = default;
    
    Function( function const pMethod ): m_pMethod( pMethod ) {}
    explicit Function( function const pMethod, C* const pClass ): m_pMethod( pMethod ), m_pClass( pClass ) {}
    Function& operator=( function const pMethod ) { m_pMethod = pMethod; return *this; }

    Function( Function const& ) = default;
    Function( Function&& ) = default;
    Function& operator=( Function const& ) = default;
    Function& operator=( Function&& ) = default;

    R operator()( Args... args ) const override { return (m_pClass->*m_pMethod)( args... ); }
    R operator()( C* const pClass, Args... args ) const { return (pClass->*m_pMethod)( args... ); }

protected:
    function m_pMethod = nullptr;
    C* m_pClass = nullptr;
};


// Method Const
template <typename C, typename R, typename ...Args>
struct Function<R(C::*)(Args...) const>: Function<R(*const)(Args...)>
{
    using function = R(C::*)(Args...) const;

    Function() = default;
    ~Function() override = default;
    
    Function( function const pMethod ): m_pMethod( pMethod ) {}
    explicit Function( function const pMethod, C const* const pClass ): m_pMethod( pMethod ), m_pClass( pClass ) {}
    Function& operator=( function const pMethod ) { m_pMethod = pMethod; return *this; }

    Function( Function const& ) = default;
    Function( Function&& ) = default;
    Function& operator=( Function const& ) = default;
    Function& operator=( Function&& ) = default;

    R operator()( Args... args ) const override { return (m_pClass->*m_pMethod)( args... ); }
    R operator()( C const* const pClass, Args... args ) const { return (pClass->*m_pMethod)( args... ); }

protected:
    function m_pMethod = nullptr;
    C const* m_pClass = nullptr;
};


#endif
//
//  utils.h
//  fbide
//
//  Created by Albert on 28/09/2013.
//  Copyright (c) 2013 Albert Varaksin. All rights reserved.
//
#pragma once

// Make unique ID
#define CONCATENATE_DETAIL(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_DETAIL(x, y)
#define MAKE_UNIQUE(x) CONCATENATE(x, __COUNTER__)


// debugging
#define LOG(_msg) std::cout << (_msg) << '\n';
#define LOG_V(_var) std::cout << #_var " = " << (_var) << '\n';


/**
 * Execute given function at scope exit
 */
template<typename Callback>
class ScopeGuard
{
    /**
     * callback to execute
     */
    Callback m_callback;
    
public:
    
    /**
     * create the guard
     */
    ScopeGuard(Callback && callback) : m_callback(callback) {}
    
    
    /**
     * Execute the callback when ScopeGuard destroyed
     */
    ~ScopeGuard()
    {
        m_callback();
    }
};

/**
 * make scoped guard object
 */
template<typename Callback>
ScopeGuard<Callback> makeScopeGuard(Callback && callback)
{
    return ScopeGuard<Callback>(std::forward<Callback>(callback));
}

/**
 * macro to define in-place scoped guard object
 */
#define SCOPED_GUARD(_callback) auto MAKE_UNIQUE(_tmp_guard) = makeScopeGuard(_callback);

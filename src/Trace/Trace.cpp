/*! ****************************************************************************
\file             Trace.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include <ctime>      // clock

#include "Trace.hpp"  // Trace

// Private Macros                         //////////////////////////////////////

  /// Shorten the length of the constructor trace lines
#define TRACE_BASE "U["<<stc<<":"<<std::clock()<<"]: "
  /// Shorten the length of operator<< trace lines
#define TRACE_LEVEL(l) "U["<<l<<":"<<std::clock()<<"]: "
  /// Max size of the buffer (in bytes) inside of Trace
#define MAX_BUFF (1<<8)

//////// Trace ////////

Trace Trace::log("Master.log");

// Public Functions                     //////////////////////////////////////

Trace::Trace(const std::string& l_Filename) : m_Buffer(), m_File(l_Filename),
  m_Level(stc), m_Empty(""), m_Mutex()
{
    // Print messages if the file is open
  if(!m_File.is_open())
  {
    std::cout.flush();
    std::cerr << TRACE_BASE
              << "ERROR: Failed to open "
              << l_Filename
              << " for logging\n";
  }

  m_File << TRACE_BASE << "Format: U[trace level:cpu clock time]\n";
}

Trace::~Trace()
{
    // Write the buffer to the output streams
  FlushBuffer();
    // Close the output file
  m_File.close();
}

Trace::TraceThread Trace::operator[](int32_t l_Level)
{
    // Lock the trace object
  m_Mutex.lock();

  m_Level = l_Level;
  log.m_Level = l_Level;
    // If level is high enough to print, then print
  if(m_Level <= MIN_TRACE_LEVEL)
  {
    if(this != &log) m_Buffer << TRACE_LEVEL(l_Level);
    log.m_Buffer << TRACE_LEVEL(l_Level);
      // If the message is an error, print appropriately
    if(m_Level == err)
    {
      if(this != &log) m_Buffer << "****ERROR****: ";
      log.m_Buffer << "****ERROR****: ";
    }
  }
    // Return TraceThread proxy object
  return TraceThread(*this, m_Mutex);
}

void Trace::flush()
{
  m_Mutex.lock();
  FlushBuffer();
  log.FlushBuffer();
  m_Mutex.unlock();
}

Trace & Trace::operator<<(ostream_manip pf)
{
  return operator<<<ostream_manip>(pf);
}

Trace & Trace::operator<<(basic_ios_manip pf)
{
  return operator<<<basic_ios_manip>(pf);
}

Trace & Trace::operator<<(ios_manip pf)
{
  return operator<<<ios_manip>(pf);
}

// Private Functions                    //////////////////////////////////////

void Trace::FlushBuffer()
{
    // Create a string that stores the string stream's string
  std::string l_str = m_Buffer.str();

  if(m_File.is_open())
  {
    m_File << l_str;
    m_File.flush();
  }

  if(this == &log)
  {
    std::clog << l_str;
    std::clog.flush();
  }
    // Clear the internal string of the stringstream object
  m_Buffer.str(m_Empty);
}

void Trace::CheckFlush()
{
  if(m_Buffer.tellp() > MAX_BUFF || m_Level == err)
  {
    FlushBuffer();
  }
}

//////// TraceThread ////////

Trace::TraceThread::TraceThread(Trace & trc, std::mutex & mtx) :
  m_Trace(trc), m_Mutex(mtx)
{
}

Trace::TraceThread::~TraceThread()
{
  m_Trace.CheckFlush();
  log.CheckFlush();
  m_Mutex.unlock();
}

Trace::TraceThread & Trace::TraceThread::operator<<(ostream_manip pf)
{
  return operator<<<ostream_manip>(pf);
}

Trace::TraceThread & Trace::TraceThread::operator<<(basic_ios_manip pf)
{
  m_Trace.operator<<<basic_ios_manip>(pf);
  log.operator<<<basic_ios_manip>(pf);
  return *this;
}

Trace::TraceThread & Trace::TraceThread::operator<<(ios_manip pf)
{
  return operator<<<ios_manip>(pf);
}

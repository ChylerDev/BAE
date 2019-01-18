/*! ****************************************************************************
\file             Trace.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

#ifndef __TRACE_HPP
#define __TRACE_HPP

// Include Files                ////////////////////////////////////////////////

#include <iostream> // cout
#include <fstream>  // ofstream
#include <sstream>  // stringstream
#include <string>   // string
#include <mutex>    // mutex

// Public Macros                ////////////////////////////////////////////////

#ifdef _DEBUG
  /// The minimum trace level to log
 #define MIN_TRACE_LEVEL frq
#else
  /// The minimum trace level to log
 #define MIN_TRACE_LEVEL stc
#endif

// Public Enums                 ////////////////////////////////////////////////

  /// Defines the different levels that the trace message can be
enum
{
  err = -1, // error, used when logging an error
  stc,      // static, used for logging messages that happen sporadictly (e.g.
            // class diagnostics on destroy)
  oft,      // often, used for logging messages that are typically logged
            // multiple times during execution
  frq,      // frequent, similar to oft, used for logging messages that are
            // typically logged many times during execution
};

// Public Objects             //////////////////////////////////////////////////

/*! ****************************************************************************
\brief
  Class defining methods that will allow the user to send messages to cout and
  a file with different status levels.

  This class has been made thread-safe through the use of a proxy class for
  the overloaded operator<<'s, as well as standard mutex use within other
  functions.

  From there, you will need to set the level of trace for your message, which
  is defined in the unnamed enum in the global namespace of Trace.hpp:

    `trace[stc]`

  And finally, to send a message to the trace system, simply use the insertion
  operators like you would for any of the std::stream classes:

    `trace[stc] << "This is a message\n";`

  You can use any of your standard iomanip parameters with the trace system,
  and they will work just like you would expect with the std::*stream classes.
*******************************************************************************/
class Trace
{
  // Members              ///////////////////////

    /// Output String Stream used to hold the buffer of output
  std::ostringstream m_Buffer;
    /// Output file stream used to print the buffer to a file
  std::ofstream m_File;
    /// The trace level of whatever current operator<< message is being inserted
  int32_t m_Level;
    /// Empty string to be used in the FlushBuffer function
  std::string m_Empty;
    /// Mutex for thread-safety
  std::mutex m_Mutex;

  class TraceThread;  // Forward reference for Trace::operator[]

public:

    /// The name of ostream manip parameters
  typedef std::ostream& (*ostream_manip)(std::ostream&);
    /// The name of basic_ios manip parameters
  typedef std::basic_ios<char>& (*basic_ios_manip)(std::ostream&);
    /// The name of ios_base manip parameters
  typedef std::ios_base& (*ios_manip)(std::ostream&);

  static Trace log;

  // Con-/De- structors   ///////////////////////

  /*! **************************************************************************
  \brief
    Constructor for the Trace class. Opens the internal file object with a
    given filename and traces some help for understanding the output of the
    trace system.

  \param filename
    The filename of the file that the trace system will output to as one of
    its output streams.
    If no filename is given, a default of "trace.log" is provided.
  *****************************************************************************/
  Trace(const std::string& filename);

  /*! **************************************************************************
  \brief
    Destructor for the Trace class. Flushes the buffer to the output stream(s)
    and closes the file stream if it's open.
  *****************************************************************************/
  ~Trace();

  // Operators            ///////////////////////

  /*! **************************************************************************
  \brief
    Singleton accessor for the Trace class. Takes in the trace level of the
    trace call and returns a proxy object that ensures thread safety and
    allowing the user to immediately call operator<< on the object to insert a
    message.

  \param level
    The trace level for the message the user is about to insert into the trace
    stream(s).

  \return
    An object that will unlock the mutex that was locked in this function, in
    its destructor.
  *****************************************************************************/
  [[nodiscard]] TraceThread operator[](int32_t level);

  // Functions            ///////////////////////

  /*! **************************************************************************
  \brief
    Flushes the buffer to the stream(s).
  *****************************************************************************/
  void flush();

private:

  friend TraceThread;

  // Operators            ///////////////////////

  /*! **************************************************************************
  \brief
    Templated operator<< function to allow any data type to be inserted into
    the output stream(s).

  \param rhs
    The data, on the right-hand side of the operator, being inserted into the
    stream(s).

  \return
    A reference to the "this" object.
  *****************************************************************************/
  template<typename T>
  inline Trace& operator<<(T rhs)
  {
    if(m_Level <= MIN_TRACE_LEVEL)
    {
      m_Buffer << rhs;  // Append the data to the end of the buffer
    }
    return *this;       // Return this
  }

  /*! **************************************************************************
  \brief
    Overloaded operator<< function to allow std::ostream manipulation
    functions to be passed to the internal string stream buffer, allowing
    formatted trace input.

  \param pf
    The ostream manip pointer being inserted.

  \return
    A reference to the "this" object.
  *****************************************************************************/
  Trace& operator<<(ostream_manip pf);

  /*! **************************************************************************
  \brief
    Overloaded operator<< function to allow std::ostream manipulation
    functions to be passed to the internal string stream buffer, allowing
    formatted trace input.

  \param pf
    The basic_ios manip pointer being inserted.

  \return
    A reference to the "this" object.
  *****************************************************************************/
  Trace& operator<<(basic_ios_manip pf);

  /*! **************************************************************************
  \brief
    Overloaded operator<< function to allow std::ostream manipulation
    functions to be passed to the internal string stream buffer, allowing
    formatted trace input.

  \param pf
    The ios_base manip pointer being inserted.

  \return
    A reference to the "this" object.
  *****************************************************************************/
  Trace& operator<<(ios_manip pf);

  // Functions            ///////////////////////

  /*! **************************************************************************
  \brief
    Flush the internal string stream buffer to cout (if compiled with _DEBUG)
    and the output file, if it is open.
  *****************************************************************************/
  void FlushBuffer();

  /*! **************************************************************************
  \brief
    To be called at the end of operator<< calls. Checks to see if the buffer
    should be flushed after the current message is traced.
  *****************************************************************************/
  void CheckFlush();

  /*! **************************************************************************
  \brief
    Proxy class to enable thread-safe Ulti::Trace. This class protects the
    insertion operator calls of Ulti::Trace from thread-intermingling by
    having Trace::operator[] lock the mutex, and then
    TraceThread::~TraceThread unlock it.

    A trace call will look like this:

    + `trace[stc] << "message\n"`;

    where:
    + `trace[stc]` - locks the mutex and sets the level, returning a
                      TraceThread object that can unlock the mutex.
    + `<< "message\n";` - traces the message.
    + and after the statement finished, at the semicolon, the TraceThread
      object will be destroyed, and the destructor will unlock the mutex,
      allowing a blocked call for trace to continue.
  *****************************************************************************/
  class TraceThread
  {
  private:

    // Members              ///////////////////////

      /// Trace's singleton reference
    Trace & m_Trace;
      /// Trace's mutex to unlock
    std::mutex & m_Mutex;

  public:

    // Con-/De- structors   ///////////////////////

    /*! ************************************************************************
    \brief
      Constructor for TraceThread class.

    \param trc
      Reference to the Trace singleton.
    \param mtx
      Reference to Trace's mutex.
    \param lvl
      The trace level.
    ***************************************************************************/
    TraceThread(Trace& trc, std::mutex& mtx);

    /*! ************************************************************************
    \brief
      Destructor for TraceThread class.
    ***************************************************************************/
    ~TraceThread();

    // Operators            ///////////////////////

    /*! ************************************************************************
    \brief
      Templated operator<< function to allow any data type to be inserted into
      the output stream(s).

    \param rhs
      The data, on the right-hand side of the operator, being inserted into
      the stream(s).

    \return
      A reference to the "this" object.
    ***************************************************************************/
    template<typename T>
    inline TraceThread& operator<<(T rhs)
    {
      if(&m_Trace != &log) m_Trace << rhs; // Trace the message
      log << rhs;

      return *this;   // Return reference to this
    }

#if 0
    /*! ************************************************************************
    \brief
      Overloaded operator<< function to allow std::ostream manipulation
      functions to be passed to Trace::operator<<, allowing formatted trace
      input.

    \param pf
      The ostream manip pointer being inserted.

    \return
      A reference to the "this" object.
    ***************************************************************************/
    TraceThread& operator<<(ostream_manip pf);

    /*! ************************************************************************
    \brief
      Overloaded operator<< function to allow std::ostream manipulation
      functions to be passed to Trace::operator<<, allowing formatted trace
      input.

    \param pf
      The basic_ios manip pointer being inserted.

    \return
      A reference to the "this" object.
    ***************************************************************************/
    TraceThread& operator<<(basic_ios_manip pf);

    /*! ************************************************************************
    \brief
      Overloaded operator<< function to allow std::ostream manipulation
      functions to be passed to Trace::operator<<, allowing formatted trace
      input.

    \param pf
      The ios_base manip pointer being inserted.

    \return
      A reference to the "this" object.
    ***************************************************************************/
    TraceThread& operator<<(ios_manip pf);
#endif

  private:

    /*! ************************************************************************
    \brief
      Copy constructor.

      NOTE TO FUTURE DEBUGGING CHYLER: COULD CAUSE DEADLOCK

    \param obj
      Object being copied.
    ***************************************************************************/
    TraceThread(TraceThread && obj) = default;

  }; // class TraceThread
}; // class Trace

#endif // __TRACE_HPP

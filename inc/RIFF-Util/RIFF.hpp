/*! ****************************************************************************
\file             RIFF-Util/RIFF.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  RIFF-Util

\copyright        Copyright © 2019 Chyler
*******************************************************************************/

#ifndef __RIFF_HPP
#define __RIFF_HPP

// Include Files                ////////////////////////////////////////////////

#include <cstdint>

#include <deque>    // deque
#include <iostream> // basic_istream, basic_ostream, basic_iostream, istreambuf_iterator
#include <map>      // map
#include <memory>   // shared_ptr
#include <sstream>  // basic_stringstream
#include <string>   // basic_string
#include <tuple>    // tuple
#include <vector>   // vector

// Public Macros                ////////////////////////////////////////////////

#ifndef CHAR_STR_TO_BYTE_STR
    /// Converts a C-string of type char to a C-string of type RIFF::byte_t
  #define CHAR_STR_TO_BYTE_STR(c_str) reinterpret_cast<::RIFF::byte_t const *>(c_str)
#endif
#ifndef CONSTRUCT_BYTE_STR
    /// Converts a C-string to a RIFF::string_t
  #define CONSTRUCT_BYTE_STR(c_str) ::RIFF::string_t(CHAR_STR_TO_BYTE_STR(c_str))
#endif

// Forward References           ////////////////////////////////////////////////

namespace RIFF
{
    /// Alias for more meaningful names
  using byte_t = uint8_t;

    /// IOStream alias using RIFF::byte_t
  using stream_t = std::basic_iostream<byte_t>;
    /// IStream alias using RIFF::byte_t
  using istream_t = std::basic_istream<byte_t>;
    /// OStream alias using RIFF::byte_t
  using ostream_t = std::basic_ostream<byte_t>;
    /// IOStream Iterator alias using RIFF::byte_t
  using istreambuf_iterator_t = std::istreambuf_iterator<byte_t>;

    /// FStream alias using RIFF::byte_t
  using fstream_t = std::basic_fstream<byte_t>;
    /// IFStream alias using RIFF::byte_t
  using ifstream_t = std::basic_ifstream<byte_t>;
    /// OFStream alias using RIFF::byte_t
  using ofstream_t = std::basic_ofstream<byte_t>;

    /// String alias using RIFF::byte_t
  using string_t = std::basic_string<byte_t>;
    /// String stream alias using RIFF::byte_t
  using stringstream_t = std::basic_stringstream<byte_t>;

    /// Vector alias using RIFF::byte_t
  using vector_t = std::vector<byte_t>;
    /// Tuple alias using RIFF::string_t and RIFF::vector_t
  using tuple_t = std::tuple<string_t, vector_t>;
    /// Deque alias using RIFF::string_t and RIFF::vector_t
  using queue_t = std::deque<tuple_t>;

    /// Alias to string_t pointer
  using p_string_t = std::shared_ptr<string_t>;
    /// Alias to queue_t pointer
  using p_queue_t = std::shared_ptr<queue_t>;
    /// Alias to uint32_t pointer
  using p_uint32_t = std::shared_ptr<uint32_t>;
} // namespace RIFF

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace RIFF
{
  /*! **************************************************************************
  \brief
    Class for writing data in the RIFF chunk format.

    This class deals with data using std::vector. This allows the user to
    extract RIFF-formatted data without using any file i/o. Of course, you can
    still read streams from the disk, but you will need to do that yourself.
    There is provided operator<< and operator>> functions to convert between
    RIFF::vector_t and RIFF::stream_t for easier reading & writing of the RIFF
    data to streams.

    Keep in mind that the order in which you pass chunks to the AddChunks method
    is the same order in which those chunks will appear in the formatted RIFF
    data, so take care of what order you pass data to the Writer. For example,
    in the WAVE audio format, the "fmt " chunk is required to come before the
    "data" chunk, so you will need to pass the writer the "fmt " chunk before
    the "data" chunk for the WAVE data to be properly read and parsed by other
    programs.

    More info about the RIFF file format can be found here:
    https://en.wikipedia.org/wiki/Resource_Interchange_File_Format
  *****************************************************************************/
  class Writer
  {
  protected:

    // Members              ///////////////////////

      /// Map consisting of chunk data mapped to chunk labels
    p_queue_t m_Chunks;
      /// The file type of the RIFF data
    p_string_t m_FileType;
      /// Reference counter for lazy copy
    p_uint32_t m_RefCount;

  public:

    // Con-/De- structors   ///////////////////////

    /*! ************************************************************************
    \brief
      Constructs a RIFF Writer object, initializing with only the file type.

    \param file_type
      The file type to be formatted, e.g. "WAVE" for .wav audio files.

      NOTE: This string MUST be exactly 4 characters long, as required by the
            RIFF standard.
    ***************************************************************************/
    Writer(string_t const & file_type);

    /*! ************************************************************************
    \brief
      Copy constructor that employs lazy-copy semantics.

    \param other
      Object being copied.
    ***************************************************************************/
    Writer(Writer const & other);

      /// Default move
    Writer(Writer &&) noexcept = default;

      /// Default destructor
    virtual ~Writer();

    // Operators            ///////////////////////

    /*! ************************************************************************
    \brief
      Assignment operator that employs lazy-copy semantics.

    \return
      *this
    ***************************************************************************/
    Writer & operator=(Writer const &);

      /// Default move-assign \return *this
    Writer & operator=(Writer &&) noexcept = default;

    // Accossors/Mutators   ///////////////////////

    // Functions            ///////////////////////

    /*! ************************************************************************
    \brief
      Adds a RIFF chunk to the Writer with the given label.

      Keep in mind that the order in which you pass chunks to this method is the
      same order in which those chunks will appear in the formatted RIFF data,
      so take care of what order you pass data to the Writer. For example, in
      the WAVE audio format, the "fmt " chunk is required to come before the
      "data" chunk, so you will need to pass the writer the "fmt " chunk before
      the "data" chunk for the WAVE data to be properly read and parsed by other
      programs.

    \param label
      The label for the chunk being written, e.g. "fmt " for the WAVE format
      chunk in .wav audio files.

      NOTE: This string MUST be exactly 4 characters long, as required by the
            RIFF standard.

    \param data
      The data to be stored within the chunk.
    ***************************************************************************/
    void AddChunk(string_t const & label, vector_t const & data);

    /*! ************************************************************************
    \brief
      Returns the formatted RIFF data.

    \return
      Vector containing the formatted RIFF data.
    ***************************************************************************/
    vector_t RIFFData() const;

  protected:

    // Con-/De- structors   ///////////////////////

    /*! ************************************************************************
    \brief
      Protected constructor to initialize the class's members with external
      shared data.

    \param shared_queue
      The shared queue for chunks.

    \param shared_string
      The shared string for file type.

    \param shared_refcount
      The shared uint32 for reference counting.
    ***************************************************************************/
    Writer(p_queue_t const & shared_queue,
           p_string_t const & shared_string,
           p_uint32_t const & shared_refcount);

    // Functions                  ///////////////////////

    /*! ************************************************************************
    \brief
      Updates the shared data of the class.

    \param shared_queue
      The shared queue for chunks.

    \param shared_string
      The shared string for file type.

    \param shared_refcount
      The shared uint32 for reference counting.
    ***************************************************************************/
    void UpdateShared(p_queue_t const & shared_queue,
                      p_string_t const & shared_string,
                      p_uint32_t const & shared_refcount);

    /*! ************************************************************************
    \brief
      Executes the copying portion of lazy-copy semantics and isolates this
      object's data from any other object that is sharing it.
    ***************************************************************************/
    virtual void LazyCopyIsolate();

  }; // class Writer

  /*! **************************************************************************
  \brief
    Class for reading data in the RIFF chunk format.

    This class deals with data using std::vector. This allows the user to
    extract RIFF-formatted data without using any file i/o. Of course, you can
    still read streams from the disk, but you will need to do that yourself.
    There is provided operator<< and operator>> functions to convert between
    RIFF::vector_t and RIFF::stream_t for easier writing of the RIFF data to
    streams.

    More info about the RIFF file format can be found here:
    https://en.wikipedia.org/wiki/Resource_Interchange_File_Format
  *****************************************************************************/
  class Reader
  {
  protected:

    // Members              ///////////////////////

      /// Map consisting of chunk data mapped to chunk labels
    p_queue_t m_Chunks;
      /// Reference counter for lazy copy
    p_uint32_t m_RefCount;

  public:

    // Con-/De- structors   ///////////////////////

    /*! ************************************************************************
    \brief
      Constructs a RIFF Reader object, initiliazing with the RIFF data and the
      expected file type.

    \param data
      The RIFF-formatted data.

    \param file_type
      The file type the RIFF data is expected to be formatted in, e.g. "WAVE"
      for .wav audio files.

      NOTE: This string MUST be exactly 4 characters long, as required by the
            RIFF standard.
    ***************************************************************************/
    Reader(vector_t const & data, string_t const & file_type);

    /*! ************************************************************************
    \brief
      Copy constructor that employs lazy-copy semantics.

    \param other
      Object being copied.
    ***************************************************************************/
    Reader(Reader const & other);

      /// Default move
    Reader(Reader &&) noexcept = default;

      /// Default destructor
    virtual ~Reader();

    // Operators            ///////////////////////

    /*! ************************************************************************
    \brief
      Assignment operator that employs lazy-copy semantics.

    \return
      *this
    ***************************************************************************/
    Reader & operator=(Reader const &);

      /// Default move-assign \return *this
    Reader & operator=(Reader &&) noexcept = default;

    // Accossors/Mutators   ///////////////////////

    // Functions            ///////////////////////

    /*! ************************************************************************
    \brief
      Returns the data for the given chunk. If the label cannot be found within
      the map, the map (and thus this function) will throw a std::out_of_range
      exception.

    \param label
      The label for the data to retrieve.

      NOTE: This string MUST be exactly 4 characters long, as required by the
            RIFF standard.

    \return
      A reference to a constant vector containing the chunk's data.
    ***************************************************************************/
    vector_t const & GetChunk(string_t const & label) const;

  protected:

    // Con-/De- structors   ///////////////////////

    /*! ************************************************************************
    \brief
      Protected constructor to initialize the class's members with external
      shared data.

    \param shared_queue
      The shared queue for chunks.

    \param shared_refcount
      The shared uint32 for reference counting.
    ***************************************************************************/
    Reader(p_queue_t const & shared_queue, p_uint32_t const & shared_refcount);

    /*! ************************************************************************
    \brief
      Updates the shared data of the class.

    \param shared_queue
      The shared queue for chunks.

    \param shared_refcount
      The shared uint32 for reference counting.
    ***************************************************************************/
    void UpdateShared(p_queue_t const & shared_queue, p_uint32_t const & shared_refcount);

    /*! ************************************************************************
    \brief
      Executes the copying portion of lazy-copy semantics and isolates this
      object's data from any other object that is sharing it.
    ***************************************************************************/
    virtual void LazyCopyIsolate();

    /*! ************************************************************************
    \brief
      Parses the vector of RIFF data and copies the chunks to the internal map.

    \param riff
      Vector of RIFF data.

    \param file_type
      The file type the RIFF data is expected to be in.
    ***************************************************************************/
    void ParseRIFF(vector_t const & riff, string_t const & file_type);

  }; // class Reader
} // namespace RIFF

// Public Functions             ////////////////////////////////////////////////

/*! ****************************************************************************
\brief
  Inserts the entire passed vector's data to into passed output stream.

\return
  The passed output stream.
*******************************************************************************/
RIFF::ostream_t & operator<<(RIFF::ostream_t &, RIFF::vector_t const &);

/*! ****************************************************************************
\brief
  Extracts the remaining data from the passed input stream into the passed
  vector.

\return
  The passed input stream.
*******************************************************************************/
RIFF::istream_t & operator>>(RIFF::istream_t &, RIFF::vector_t &);

#endif // __RIFF_HPP

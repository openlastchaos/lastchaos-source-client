#ifndef SE_INCL_BUFFER_H
#define SE_INCL_BUFFER_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <Engine/Base/Timer.h>

/*
 * Class for buffering data streams at byte level.
 */
class CBuffer {
public:
  SLONG bu_slAllocationStep;  // how many bytes to add when buffer overflows
  SLONG bu_slWriteOffset; // where to write to next
  SLONG bu_slReadOffset;  // where to read from next

  SLONG bu_slFree;        // number of bytes free
  SLONG bu_slSize;        // current buffer size
  UBYTE *bu_pubBuffer;    // buffer memory

  // default constructor
  CBuffer(void);
  // destructor
  ~CBuffer(void);

  // how many bytes of data is in the buffer
  SLONG GetUsedSize();

  // free buffer
  virtual void Clear(void);
  // expand buffer to be given number of bytes in size
  virtual void Expand(SLONG slNewSize);
  // set how many bytes to add when buffer overflows
  void SetAllocationStep(SLONG slStep);
  // read bytes from buffer
  virtual SLONG ReadBytes(void *pv, SLONG slSize);
  // skip bytes from buffer (read without actually reading)
  SLONG SkipBytes(SLONG slSize);
  // read bytes from buffer to stream
  SLONG ReadBytesToStream(CTStream &strm, SLONG slSize);
  // unread bytes from buffer
  void UnreadBytes(SLONG slSize);
  // check how many bytes are there to read
  SLONG QueryReadBytes(void);
  // write bytes to buffer
  virtual void WriteBytes(const void *pv, SLONG slSize);
  // move all data from another buffer to this one
  void MoveBuffer(CBuffer &buFrom);
};

// data for limiting bandwidth/lantency and calculating statistics in block-buffers
class CBlockBufferStats {
public:
  FLOAT bbs_fLatencyLimit;    // minimum latency in seconds
  FLOAT bbs_fLatencyVariation;// additional latency variation
  FLOAT bbs_fBandwidthLimit;  // maximum bandwidth in bps (bits per second)
  CTimerValue bbs_tvTimeUsed; // next point in time free for data receiving

  void Clear(void);
  // get time when block of given size will be finished if started now
  CTimerValue GetBlockFinalTime(SLONG slSize);
};

/*
 * Class for buffering data streams at block level.
 */
class CBlockBuffer : public CBuffer {
public:
  CBlockBufferStats *bb_pbbsStats;   // for bandwidth/latency stats and limits
  SLONG bb_slBlockSizeRead;     // block size left for reading (0 if not inside block)
  SLONG bb_slBlockSizeWrite;    // block size left for writing (0 if not inside block)

  // default constructor
  CBlockBuffer(void);
  // destructor
  ~CBlockBuffer(void);
  // free buffer
  void Clear(void);

  // read one block if possible
  BOOL ReadBlock(void *pv, SLONG &slSize);
  // read one block from buffer to stream
  BOOL ReadBlockToStream(CTStream &strm);
  // write one block
  void WriteBlock(const void *pv, SLONG slSize);
  // unread one block
  void UnreadBlock(SLONG slSize);

  // read raw block data
  SLONG ReadRawBlock(void *pv, SLONG slSize);
  // write raw block data
  void WriteRawBlock(const void *pv, SLONG slSize);
  // unread raw block data
  void UnreadRawBlock(SLONG slSize);
  // move all data from another buffer to this one
  void MoveBlockBuffer(CBlockBuffer &buFrom);
  // peek sizes of next block
  void PeekBlockSize(SLONG &slExpectedSize, SLONG &slReceivedSoFar);
};


#endif  /* include-once check. */


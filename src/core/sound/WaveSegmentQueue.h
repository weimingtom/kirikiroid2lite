//---------------------------------------------------------------------------
/*
	Risa
	 stands for "Risa Is a Stagecraft Architecture"
	Copyright (C) 2000 W.Dee <dee@kikyou.info> and contributors

	See details of license at "license.txt"
*/
//---------------------------------------------------------------------------
//! @file
//! @brief
//---------------------------------------------------------------------------
#ifndef WAVESEGMENTH
#define WAVESEGMENTH


#include <deque>


//---------------------------------------------------------------------------
//! @brief
//---------------------------------------------------------------------------
struct tTVPWaveSegment
{
	tTVPWaveSegment(tjs_int64 start, tjs_int64 length)
		{ Start = start; Length = FilteredLength = length; }
	tTVPWaveSegment(tjs_int64 start, tjs_int64 length, tjs_int64 filteredlength)
		{ Start = start; Length = length; FilteredLength = filteredlength; }
	tjs_int64 Start; 
	tjs_int64 Length; 
	tjs_int64 FilteredLength; 
};
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//! @brief 再生ラベル情報
//---------------------------------------------------------------------------
struct tTVPWaveLabel
{
	//! @brief
	tjs_int64 Position; 
	ttstr Name; //!<
	tjs_int Offset;
		/*!<
			@note
			This member will be set in tTVPWaveLoopManager::Render,
			and will contain the sample granule offset from first decoding
			point at call of tTVPWaveLoopManager::Render().
		*/

#ifdef TVP_IN_LOOP_TUNER
	// these are only used by the loop tuner
	tjs_int NameWidth; // display name width
	tjs_int Index; // index
#endif

	struct tSortByPositionFuncObj
	{
		bool operator()(
			const tTVPWaveLabel &lhs,
			const tTVPWaveLabel &rhs) const
		{
			return lhs.Position < rhs.Position;
		}
	};

	struct tSortByOffsetFuncObj
	{
		bool operator()(
			const tTVPWaveLabel &lhs,
			const tTVPWaveLabel &rhs) const
		{
			return lhs.Offset < rhs.Offset;
		}
	};

#ifdef TVP_IN_LOOP_TUNER
	struct tSortByIndexFuncObj
	{
		bool operator()(
			const tTVPWaveLabel &lhs,
			const tTVPWaveLabel &rhs) const
		{
			return lhs.Index < rhs.Index;
		}
	};
#endif

	//! @brief
	tTVPWaveLabel()
	{
		Position = 0;
		Offset = 0;
#ifdef TVP_IN_LOOP_TUNER
		NameWidth = 0;
		Index = 0;
#endif
	}

	tTVPWaveLabel(tjs_int64 position, const ttstr & name, tjs_int offset)
		: Position(position), Name(name), Offset(offset)
	{
#ifdef TVP_IN_LOOP_TUNER
		NameWidth = 0;
		Index = 0;
#endif
	}
};
//---------------------------------------------------------------------------
bool inline operator < (const tTVPWaveLabel & lhs, const tTVPWaveLabel & rhs)
{
	return lhs.Position < rhs.Position;
}
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
//! @brief
//---------------------------------------------------------------------------
class tTVPWaveSegmentQueue
{
	std::deque<tTVPWaveSegment> Segments;
	std::deque<tTVPWaveLabel> Labels;

public:
	void Clear();

	const std::deque<tTVPWaveSegment> & GetSegments() const { return Segments; }

	const std::deque<tTVPWaveLabel> & GetLabels() const { return Labels; }

	void Enqueue(const tTVPWaveSegmentQueue & queue);

	void Enqueue(const tTVPWaveSegment & segment);

	void Enqueue(const tTVPWaveLabel & Label);

	void Enqueue(const std::deque<tTVPWaveSegment> & segments);

	void Enqueue(const std::deque<tTVPWaveLabel> & Labels);

	void Dequeue(tTVPWaveSegmentQueue & dest, tjs_int64 length);

	tjs_int64 GetFilteredLength() const;

	void Scale(tjs_int64 new_total_length);

	tjs_int64 FilteredPositionToDecodePosition(tjs_int64 pos) const;
};
//---------------------------------------------------------------------------

#endif


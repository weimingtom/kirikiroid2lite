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
#include "tjsCommHead.h"

#include "WaveSegmentQueue.h"


//---------------------------------------------------------------------------
void tTVPWaveSegmentQueue::Clear()
{
	Labels.clear();
	Segments.clear();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void tTVPWaveSegmentQueue::Enqueue(const tTVPWaveSegmentQueue & queue)
{
	Enqueue(queue.Labels);
	Enqueue(queue.Segments);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void tTVPWaveSegmentQueue::Enqueue(const tTVPWaveSegment & segment)
{
	if(Segments.size() > 0)
	{
		tTVPWaveSegment & last = Segments.back();
		if(last.Start + last.Length == segment.Start &&
			(double)last.FilteredLength / last.Length ==
			(double)segment.FilteredLength / segment.Length)
		{
			last.FilteredLength += segment.FilteredLength;
			last.Length += segment.Length;
			return ;
		}
	}

	Segments.push_back(segment);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void tTVPWaveSegmentQueue::Enqueue(const tTVPWaveLabel & Label)
{
	Labels.push_back(Label);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void tTVPWaveSegmentQueue::Enqueue(const std::deque<tTVPWaveSegment> & segments)
{
	for(std::deque<tTVPWaveSegment>::const_iterator i = segments.begin();
		i != segments.end(); i++)
		Enqueue(*i);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void tTVPWaveSegmentQueue::Enqueue(const std::deque<tTVPWaveLabel> & Labels)
{
	tjs_int64 Label_offset = GetFilteredLength();

	for(std::deque<tTVPWaveLabel>::const_iterator i = Labels.begin();
		i != Labels.end(); i++)
	{
		tTVPWaveLabel one_Label(*i);
		one_Label.Offset += static_cast<tjs_int>(Label_offset);
		Enqueue(one_Label);
	}
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void tTVPWaveSegmentQueue::Dequeue(tTVPWaveSegmentQueue & dest, tjs_int64 length)
{
	tjs_int64 remain;
	dest.Clear();

	remain = length;
	while(Segments.size() > 0 && remain > 0)
	{
		if(Segments.front().FilteredLength <= remain)
		{
			remain -= Segments.front().FilteredLength;
			dest.Enqueue(Segments.front());
			Segments.pop_front();
		}
		else
		{
			tjs_int64 newlength =
				static_cast<tjs_int64>(
					(double)Segments.front().Length / (double)Segments.front().FilteredLength * remain);
			if(newlength > 0)
				dest.Enqueue(tTVPWaveSegment(Segments.front().Start, newlength, remain));

			Segments.front().Start += newlength;
			Segments.front().Length -= newlength;
			Segments.front().FilteredLength -= remain;
			if(Segments.front().Length == 0 || Segments.front().FilteredLength == 0)
			{
				Segments.pop_front();
			}
			remain = 0;
		}
	}

	size_t Labels_to_dequeue = 0;
	for(std::deque<tTVPWaveLabel>::iterator i = Labels.begin();
		i != Labels.end(); i++)
	{
		tjs_int64 newoffset = i->Offset - length;
		if(newoffset < 0)
		{
			dest.Enqueue(*i);
			Labels_to_dequeue ++;
		}
		else
		{
			i->Offset = static_cast<tjs_int>(newoffset);
		}
	}

	while(Labels_to_dequeue--) Labels.pop_front();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
tjs_int64 tTVPWaveSegmentQueue::GetFilteredLength() const
{
	tjs_int64 length = 0;
	for(std::deque<tTVPWaveSegment>::const_iterator i = Segments.begin();
		i != Segments.end(); i++)
		length += i->FilteredLength;

	return length;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void tTVPWaveSegmentQueue::Scale(tjs_int64 new_total_filtered_length)
{
	tjs_int64 total_length_was = GetFilteredLength();

	if(total_length_was == 0) return;

	tjs_int64 offset_was = 0;
	tjs_int64 offset_is = 0;

	for(std::deque<tTVPWaveSegment>::iterator i = Segments.begin();
		i != Segments.end(); i++)
	{
		tjs_int64 old_end = offset_was + i->FilteredLength;
		offset_was += i->FilteredLength;

		double ratio = static_cast<double>(old_end) /
						static_cast<double>(total_length_was);

		tjs_int64 new_end = static_cast<tjs_int64>(ratio * new_total_filtered_length);

		i->FilteredLength = new_end - offset_is;

		offset_is += i->FilteredLength;
	}

	for(std::deque<tTVPWaveSegment>::iterator i = Segments.begin();
		i != Segments.end() ; )
	{
		if(i->FilteredLength == 0 || i->Length == 0)
			i = Segments.erase(i);
		else
			i++;
	}

	double ratio = (double)new_total_filtered_length / (double)total_length_was;
	for(std::deque<tTVPWaveLabel>::iterator i = Labels.begin();
		i != Labels.end(); i++)
	{
		i->Offset = static_cast<tjs_int64>(i->Offset * ratio);
	}
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
tjs_int64 tTVPWaveSegmentQueue::FilteredPositionToDecodePosition(tjs_int64 pos) const
{
	tjs_int64 offset_filtered = 0;

	for(std::deque<tTVPWaveSegment>::const_iterator i = Segments.begin();
		i != Segments.end(); i++)
	{
		if(offset_filtered <= pos && pos < offset_filtered + i->FilteredLength)
		{
			return (tjs_int64)(i->Start + (pos - offset_filtered) *
				(double)i->Length / (double)i->FilteredLength );
		}

		offset_filtered += i->FilteredLength;
	}

	if(pos<0) return 0;
	if(Segments.size() == 0) return 0;
	return Segments[Segments.size()-1].Start + Segments[Segments.size()-1].Length;
}
//---------------------------------------------------------------------------



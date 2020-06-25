#ifndef TVP_IWAVEUNPACKER_DEFINED
#define TVP_IWAVEUNPACKER_DEFINED
class IWaveUnpacker
{
public:
// IUnknown �h���N���X�ł͂Ȃ��̂Œ���
	virtual ULONG STDMETHODCALLTYPE AddRef(void) = 0;
	virtual ULONG STDMETHODCALLTYPE Release(void) = 0;

// IWaveUnpacker
	virtual HRESULT STDMETHODCALLTYPE GetTypeName(char *buf,long buflen)=0;
		/*
			buf �ɁA���� Wave �`����\��������� *buf �ɐݒ肵�Ă��������B
			buflen �́Abuf �Ɋm�ۂ��ꂽ������ŁAnull terminater ���܂ނ̂�
			���ӁB
		*/

	virtual HRESULT STDMETHODCALLTYPE GetWaveFormat(long *samplepersec,
		long *channels,long *bitspersample)=0;
		/*
			�o�͂��� Wave �̌`���� *samplepersec, *channels, *bitspersample ��
			�Ԃ��Ă��������B
		*/

	virtual HRESULT STDMETHODCALLTYPE Render(void *buffer,long bufsize,
		long *numwrite) =0;
		/*
			�f�R�[�h���Ă��������B
			bufsize �ɂ� buffer �̃T�C�Y���o�C�g�P�ʂŎw�肳��܂��B
			numwrite �ɂ́A�o�b�t�@�ɏ����ꂽ�f�[�^�̐����o�C�g�P�ʂŕԂ��܂��B
			�������AWaveUnpacker �́Anumwrite<bufsize �̏ꍇ�́A�c���
			0 �Ŗ��߂Ă��������B
		*/
	
	virtual HRESULT STDMETHODCALLTYPE GetLength(long *length)=0;
		/*
			�f�[�^���� ms �P�ʂ� *length �ɕԂ��Ă��������B
			�Ή��ł��Ȃ��ꍇ�� E_NOTIMPL ��Ԃ��Ă��������B���̏ꍇ��
			WaveSoundBuffer �� totalTime �v���p�e�B�� 0 ��\���悤�ɂȂ�܂��B
		*/

	virtual HRESULT STDMETHODCALLTYPE GetCurrentPosition(long *pos)=0;
		/*
			���݂̃f�R�[�h�ʒu�� *pos �ɕԂ��Ă��������B
			�Ή��ł��Ȃ��ꍇ�� E_NOTIMPL ��Ԃ��Ă��������B���̏ꍇ��
			WaveSoundBuffer �� position �v���p�e�B�͈Ӗ��̂Ȃ����l��
			�����悤�ɂȂ�܂��B
		*/

	virtual HRESULT STDMETHODCALLTYPE SetCurrentPosition(long pos)=0;
		/*
			���݂̃f�R�[�h�ʒu��ݒ肵�Ă��������Bpos �� ms �P�ʂł�
			�ʒu�ł��B
			�Œ�ł� pos=0 �Ƃ��ČĂ΂ꂽ�Ƃ��ɁA�擪�ւ̊����߂���
			�o���悤�ɂ��Ă��������B

			���̂ق��̏ꍇ�A�Ή��ł��Ȃ��ꍇ�� E_NOTIMPL ��Ԃ��Ă��������B
			���̏ꍇ��WaveSoundBuffer �� position �v���p�e�B�ւ̑���͖�������܂��B
		*/

	virtual HRESULT STDMETHODCALLTYPE Invoke(); // reserved
};

#endif
//---------------------------------------------------------------------------

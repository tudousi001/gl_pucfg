/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Mon Nov 05 20:33:01 2007
 */
/* Compiler settings for D:\vc\HCView\IBaseNetView.idl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __IBaseNetView_h__
#define __IBaseNetView_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IBaseNetView_FWD_DEFINED__
#define __IBaseNetView_FWD_DEFINED__
typedef interface IBaseNetView IBaseNetView;
#endif 	/* __IBaseNetView_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IBaseNetView_INTERFACE_DEFINED__
#define __IBaseNetView_INTERFACE_DEFINED__

/* interface IBaseNetView */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IBaseNetView;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F3C4C7BB-1D4B-4983-973C-88F39787B8AD")
    IBaseNetView : public IDispatch
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Initial( 
            /* [in] */ long hWnd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Free( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetVersion( 
            /* [out] */ long __RPC_FAR *lVer) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueryFunction( 
            /* [in] */ BSTR sFunctionName,
            /* [out] */ long __RPC_FAR *lCanUse) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetParameter( 
            /* [in] */ long hHandle,
            /* [in] */ long lType,
            /* [in] */ long lPara1,
            /* [in] */ long lPara2) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetParameter( 
            /* [in] */ long hHandle,
            /* [in] */ long lType,
            /* [out] */ long __RPC_FAR *pPara1,
            /* [out] */ long __RPC_FAR *pPara2) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetCallBack( 
            /* [in] */ long hHandle,
            /* [in] */ long callbackptr) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetColor( 
            /* [in] */ long hHandle,
            /* [in] */ long lBrightness,
            /* [in] */ long lContrast,
            /* [in] */ long lSaturation,
            /* [in] */ long lHue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetColor( 
            /* [in] */ long hHandle,
            /* [out] */ long __RPC_FAR *pBrightness,
            /* [out] */ long __RPC_FAR *pContrast,
            /* [out] */ long __RPC_FAR *pSaturation,
            /* [out] */ long __RPC_FAR *pHue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetVolume( 
            /* [in] */ long hHandle,
            /* [in] */ long lVolume) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetVolume( 
            /* [in] */ long hHandle,
            /* [out] */ long __RPC_FAR *pVolume) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PlaySound( 
            /* [in] */ long hHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE StopSound( 
            /* [in] */ long hHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Play( 
            /* [in] */ long hHandle,
            /* [in] */ long hWnd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Stop( 
            /* [in] */ long hHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Pause( 
            /* [in] */ long hHandle,
            /* [in] */ long lPause) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RefreshSurface( 
            /* [in] */ long hHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CapPic( 
            /* [in] */ long hHandle,
            /* [in] */ BSTR sFileName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetPlayDirection( 
            /* [in] */ long lDirection) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OpenFile( 
            /* [out] */ long __RPC_FAR *phHandle,
            /* [in] */ BSTR sFileName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CloseFile( 
            /* [in] */ long hHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetPlayPos( 
            /* [in] */ long hHandle,
            /* [in] */ long lPos) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPlayPos( 
            /* [in] */ long hHandle,
            /* [out] */ long __RPC_FAR *pPos) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Fast( 
            /* [in] */ long hHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Slow( 
            /* [in] */ long hHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetFileEndMsgWnd( 
            /* [in] */ long hHandle,
            /* [in] */ long hWnd,
            /* [in] */ long lMsg) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OpenStream( 
            /* [out] */ long __RPC_FAR *phHandle,
            /* [in] */ long __RPC_FAR *pFileHeadBuf,
            /* [in] */ long lSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CloseStream( 
            /* [in] */ long hHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE InputData( 
            /* [in] */ long hHandle,
            /* [in] */ long __RPC_FAR *pBuf,
            /* [in] */ long lSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OpenAudio( 
            long __RPC_FAR *phHandle,
            long wavein,
            long waveout) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CloseAudio( 
            long hHandle) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IBaseNetViewVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IBaseNetView __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IBaseNetView __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IBaseNetView __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IBaseNetView __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IBaseNetView __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IBaseNetView __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IBaseNetView __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Initial )( 
            IBaseNetView __RPC_FAR * This,
            /* [in] */ long hWnd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Free )( 
            IBaseNetView __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetVersion )( 
            IBaseNetView __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *lVer);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryFunction )( 
            IBaseNetView __RPC_FAR * This,
            /* [in] */ BSTR sFunctionName,
            /* [out] */ long __RPC_FAR *lCanUse);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetParameter )( 
            IBaseNetView __RPC_FAR * This,
            /* [in] */ long hHandle,
            /* [in] */ long lType,
            /* [in] */ long lPara1,
            /* [in] */ long lPara2);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetParameter )( 
            IBaseNetView __RPC_FAR * This,
            /* [in] */ long hHandle,
            /* [in] */ long lType,
            /* [out] */ long __RPC_FAR *pPara1,
            /* [out] */ long __RPC_FAR *pPara2);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetCallBack )( 
            IBaseNetView __RPC_FAR * This,
            /* [in] */ long hHandle,
            /* [in] */ long callbackptr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetColor )( 
            IBaseNetView __RPC_FAR * This,
            /* [in] */ long hHandle,
            /* [in] */ long lBrightness,
            /* [in] */ long lContrast,
            /* [in] */ long lSaturation,
            /* [in] */ long lHue);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetColor )( 
            IBaseNetView __RPC_FAR * This,
            /* [in] */ long hHandle,
            /* [out] */ long __RPC_FAR *pBrightness,
            /* [out] */ long __RPC_FAR *pContrast,
            /* [out] */ long __RPC_FAR *pSaturation,
            /* [out] */ long __RPC_FAR *pHue);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetVolume )( 
            IBaseNetView __RPC_FAR * This,
            /* [in] */ long hHandle,
            /* [in] */ long lVolume);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetVolume )( 
            IBaseNetView __RPC_FAR * This,
            /* [in] */ long hHandle,
            /* [out] */ long __RPC_FAR *pVolume);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PlaySound )( 
            IBaseNetView __RPC_FAR * This,
            /* [in] */ long hHandle);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StopSound )( 
            IBaseNetView __RPC_FAR * This,
            /* [in] */ long hHandle);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Play )( 
            IBaseNetView __RPC_FAR * This,
            /* [in] */ long hHandle,
            /* [in] */ long hWnd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Stop )( 
            IBaseNetView __RPC_FAR * This,
            /* [in] */ long hHandle);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Pause )( 
            IBaseNetView __RPC_FAR * This,
            /* [in] */ long hHandle,
            /* [in] */ long lPause);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RefreshSurface )( 
            IBaseNetView __RPC_FAR * This,
            /* [in] */ long hHandle);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CapPic )( 
            IBaseNetView __RPC_FAR * This,
            /* [in] */ long hHandle,
            /* [in] */ BSTR sFileName);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetPlayDirection )( 
            IBaseNetView __RPC_FAR * This,
            /* [in] */ long lDirection);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OpenFile )( 
            IBaseNetView __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *phHandle,
            /* [in] */ BSTR sFileName);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CloseFile )( 
            IBaseNetView __RPC_FAR * This,
            /* [in] */ long hHandle);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetPlayPos )( 
            IBaseNetView __RPC_FAR * This,
            /* [in] */ long hHandle,
            /* [in] */ long lPos);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetPlayPos )( 
            IBaseNetView __RPC_FAR * This,
            /* [in] */ long hHandle,
            /* [out] */ long __RPC_FAR *pPos);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Fast )( 
            IBaseNetView __RPC_FAR * This,
            /* [in] */ long hHandle);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Slow )( 
            IBaseNetView __RPC_FAR * This,
            /* [in] */ long hHandle);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetFileEndMsgWnd )( 
            IBaseNetView __RPC_FAR * This,
            /* [in] */ long hHandle,
            /* [in] */ long hWnd,
            /* [in] */ long lMsg);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OpenStream )( 
            IBaseNetView __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *phHandle,
            /* [in] */ long __RPC_FAR *pFileHeadBuf,
            /* [in] */ long lSize);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CloseStream )( 
            IBaseNetView __RPC_FAR * This,
            /* [in] */ long hHandle);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InputData )( 
            IBaseNetView __RPC_FAR * This,
            /* [in] */ long hHandle,
            /* [in] */ long __RPC_FAR *pBuf,
            /* [in] */ long lSize);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OpenAudio )( 
            IBaseNetView __RPC_FAR * This,
            long __RPC_FAR *phHandle,
            long wavein,
            long waveout);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CloseAudio )( 
            IBaseNetView __RPC_FAR * This,
            long hHandle);
        
        END_INTERFACE
    } IBaseNetViewVtbl;

    interface IBaseNetView
    {
        CONST_VTBL struct IBaseNetViewVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBaseNetView_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IBaseNetView_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IBaseNetView_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IBaseNetView_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IBaseNetView_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IBaseNetView_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IBaseNetView_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IBaseNetView_Initial(This,hWnd)	\
    (This)->lpVtbl -> Initial(This,hWnd)

#define IBaseNetView_Free(This)	\
    (This)->lpVtbl -> Free(This)

#define IBaseNetView_GetVersion(This,lVer)	\
    (This)->lpVtbl -> GetVersion(This,lVer)

#define IBaseNetView_QueryFunction(This,sFunctionName,lCanUse)	\
    (This)->lpVtbl -> QueryFunction(This,sFunctionName,lCanUse)

#define IBaseNetView_SetParameter(This,hHandle,lType,lPara1,lPara2)	\
    (This)->lpVtbl -> SetParameter(This,hHandle,lType,lPara1,lPara2)

#define IBaseNetView_GetParameter(This,hHandle,lType,pPara1,pPara2)	\
    (This)->lpVtbl -> GetParameter(This,hHandle,lType,pPara1,pPara2)

#define IBaseNetView_SetCallBack(This,hHandle,callbackptr)	\
    (This)->lpVtbl -> SetCallBack(This,hHandle,callbackptr)

#define IBaseNetView_SetColor(This,hHandle,lBrightness,lContrast,lSaturation,lHue)	\
    (This)->lpVtbl -> SetColor(This,hHandle,lBrightness,lContrast,lSaturation,lHue)

#define IBaseNetView_GetColor(This,hHandle,pBrightness,pContrast,pSaturation,pHue)	\
    (This)->lpVtbl -> GetColor(This,hHandle,pBrightness,pContrast,pSaturation,pHue)

#define IBaseNetView_SetVolume(This,hHandle,lVolume)	\
    (This)->lpVtbl -> SetVolume(This,hHandle,lVolume)

#define IBaseNetView_GetVolume(This,hHandle,pVolume)	\
    (This)->lpVtbl -> GetVolume(This,hHandle,pVolume)

#define IBaseNetView_PlaySound(This,hHandle)	\
    (This)->lpVtbl -> PlaySound(This,hHandle)

#define IBaseNetView_StopSound(This,hHandle)	\
    (This)->lpVtbl -> StopSound(This,hHandle)

#define IBaseNetView_Play(This,hHandle,hWnd)	\
    (This)->lpVtbl -> Play(This,hHandle,hWnd)

#define IBaseNetView_Stop(This,hHandle)	\
    (This)->lpVtbl -> Stop(This,hHandle)

#define IBaseNetView_Pause(This,hHandle,lPause)	\
    (This)->lpVtbl -> Pause(This,hHandle,lPause)

#define IBaseNetView_RefreshSurface(This,hHandle)	\
    (This)->lpVtbl -> RefreshSurface(This,hHandle)

#define IBaseNetView_CapPic(This,hHandle,sFileName)	\
    (This)->lpVtbl -> CapPic(This,hHandle,sFileName)

#define IBaseNetView_SetPlayDirection(This,lDirection)	\
    (This)->lpVtbl -> SetPlayDirection(This,lDirection)

#define IBaseNetView_OpenFile(This,phHandle,sFileName)	\
    (This)->lpVtbl -> OpenFile(This,phHandle,sFileName)

#define IBaseNetView_CloseFile(This,hHandle)	\
    (This)->lpVtbl -> CloseFile(This,hHandle)

#define IBaseNetView_SetPlayPos(This,hHandle,lPos)	\
    (This)->lpVtbl -> SetPlayPos(This,hHandle,lPos)

#define IBaseNetView_GetPlayPos(This,hHandle,pPos)	\
    (This)->lpVtbl -> GetPlayPos(This,hHandle,pPos)

#define IBaseNetView_Fast(This,hHandle)	\
    (This)->lpVtbl -> Fast(This,hHandle)

#define IBaseNetView_Slow(This,hHandle)	\
    (This)->lpVtbl -> Slow(This,hHandle)

#define IBaseNetView_SetFileEndMsgWnd(This,hHandle,hWnd,lMsg)	\
    (This)->lpVtbl -> SetFileEndMsgWnd(This,hHandle,hWnd,lMsg)

#define IBaseNetView_OpenStream(This,phHandle,pFileHeadBuf,lSize)	\
    (This)->lpVtbl -> OpenStream(This,phHandle,pFileHeadBuf,lSize)

#define IBaseNetView_CloseStream(This,hHandle)	\
    (This)->lpVtbl -> CloseStream(This,hHandle)

#define IBaseNetView_InputData(This,hHandle,pBuf,lSize)	\
    (This)->lpVtbl -> InputData(This,hHandle,pBuf,lSize)

#define IBaseNetView_OpenAudio(This,phHandle,wavein,waveout)	\
    (This)->lpVtbl -> OpenAudio(This,phHandle,wavein,waveout)

#define IBaseNetView_CloseAudio(This,hHandle)	\
    (This)->lpVtbl -> CloseAudio(This,hHandle)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IBaseNetView_Initial_Proxy( 
    IBaseNetView __RPC_FAR * This,
    /* [in] */ long hWnd);


void __RPC_STUB IBaseNetView_Initial_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBaseNetView_Free_Proxy( 
    IBaseNetView __RPC_FAR * This);


void __RPC_STUB IBaseNetView_Free_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBaseNetView_GetVersion_Proxy( 
    IBaseNetView __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *lVer);


void __RPC_STUB IBaseNetView_GetVersion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBaseNetView_QueryFunction_Proxy( 
    IBaseNetView __RPC_FAR * This,
    /* [in] */ BSTR sFunctionName,
    /* [out] */ long __RPC_FAR *lCanUse);


void __RPC_STUB IBaseNetView_QueryFunction_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBaseNetView_SetParameter_Proxy( 
    IBaseNetView __RPC_FAR * This,
    /* [in] */ long hHandle,
    /* [in] */ long lType,
    /* [in] */ long lPara1,
    /* [in] */ long lPara2);


void __RPC_STUB IBaseNetView_SetParameter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBaseNetView_GetParameter_Proxy( 
    IBaseNetView __RPC_FAR * This,
    /* [in] */ long hHandle,
    /* [in] */ long lType,
    /* [out] */ long __RPC_FAR *pPara1,
    /* [out] */ long __RPC_FAR *pPara2);


void __RPC_STUB IBaseNetView_GetParameter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBaseNetView_SetCallBack_Proxy( 
    IBaseNetView __RPC_FAR * This,
    /* [in] */ long hHandle,
    /* [in] */ long callbackptr);


void __RPC_STUB IBaseNetView_SetCallBack_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBaseNetView_SetColor_Proxy( 
    IBaseNetView __RPC_FAR * This,
    /* [in] */ long hHandle,
    /* [in] */ long lBrightness,
    /* [in] */ long lContrast,
    /* [in] */ long lSaturation,
    /* [in] */ long lHue);


void __RPC_STUB IBaseNetView_SetColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBaseNetView_GetColor_Proxy( 
    IBaseNetView __RPC_FAR * This,
    /* [in] */ long hHandle,
    /* [out] */ long __RPC_FAR *pBrightness,
    /* [out] */ long __RPC_FAR *pContrast,
    /* [out] */ long __RPC_FAR *pSaturation,
    /* [out] */ long __RPC_FAR *pHue);


void __RPC_STUB IBaseNetView_GetColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBaseNetView_SetVolume_Proxy( 
    IBaseNetView __RPC_FAR * This,
    /* [in] */ long hHandle,
    /* [in] */ long lVolume);


void __RPC_STUB IBaseNetView_SetVolume_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBaseNetView_GetVolume_Proxy( 
    IBaseNetView __RPC_FAR * This,
    /* [in] */ long hHandle,
    /* [out] */ long __RPC_FAR *pVolume);


void __RPC_STUB IBaseNetView_GetVolume_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBaseNetView_PlaySound_Proxy( 
    IBaseNetView __RPC_FAR * This,
    /* [in] */ long hHandle);


void __RPC_STUB IBaseNetView_PlaySound_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBaseNetView_StopSound_Proxy( 
    IBaseNetView __RPC_FAR * This,
    /* [in] */ long hHandle);


void __RPC_STUB IBaseNetView_StopSound_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBaseNetView_Play_Proxy( 
    IBaseNetView __RPC_FAR * This,
    /* [in] */ long hHandle,
    /* [in] */ long hWnd);


void __RPC_STUB IBaseNetView_Play_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBaseNetView_Stop_Proxy( 
    IBaseNetView __RPC_FAR * This,
    /* [in] */ long hHandle);


void __RPC_STUB IBaseNetView_Stop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBaseNetView_Pause_Proxy( 
    IBaseNetView __RPC_FAR * This,
    /* [in] */ long hHandle,
    /* [in] */ long lPause);


void __RPC_STUB IBaseNetView_Pause_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBaseNetView_RefreshSurface_Proxy( 
    IBaseNetView __RPC_FAR * This,
    /* [in] */ long hHandle);


void __RPC_STUB IBaseNetView_RefreshSurface_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBaseNetView_CapPic_Proxy( 
    IBaseNetView __RPC_FAR * This,
    /* [in] */ long hHandle,
    /* [in] */ BSTR sFileName);


void __RPC_STUB IBaseNetView_CapPic_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBaseNetView_SetPlayDirection_Proxy( 
    IBaseNetView __RPC_FAR * This,
    /* [in] */ long lDirection);


void __RPC_STUB IBaseNetView_SetPlayDirection_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBaseNetView_OpenFile_Proxy( 
    IBaseNetView __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *phHandle,
    /* [in] */ BSTR sFileName);


void __RPC_STUB IBaseNetView_OpenFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBaseNetView_CloseFile_Proxy( 
    IBaseNetView __RPC_FAR * This,
    /* [in] */ long hHandle);


void __RPC_STUB IBaseNetView_CloseFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBaseNetView_SetPlayPos_Proxy( 
    IBaseNetView __RPC_FAR * This,
    /* [in] */ long hHandle,
    /* [in] */ long lPos);


void __RPC_STUB IBaseNetView_SetPlayPos_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBaseNetView_GetPlayPos_Proxy( 
    IBaseNetView __RPC_FAR * This,
    /* [in] */ long hHandle,
    /* [out] */ long __RPC_FAR *pPos);


void __RPC_STUB IBaseNetView_GetPlayPos_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBaseNetView_Fast_Proxy( 
    IBaseNetView __RPC_FAR * This,
    /* [in] */ long hHandle);


void __RPC_STUB IBaseNetView_Fast_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBaseNetView_Slow_Proxy( 
    IBaseNetView __RPC_FAR * This,
    /* [in] */ long hHandle);


void __RPC_STUB IBaseNetView_Slow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBaseNetView_SetFileEndMsgWnd_Proxy( 
    IBaseNetView __RPC_FAR * This,
    /* [in] */ long hHandle,
    /* [in] */ long hWnd,
    /* [in] */ long lMsg);


void __RPC_STUB IBaseNetView_SetFileEndMsgWnd_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBaseNetView_OpenStream_Proxy( 
    IBaseNetView __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *phHandle,
    /* [in] */ long __RPC_FAR *pFileHeadBuf,
    /* [in] */ long lSize);


void __RPC_STUB IBaseNetView_OpenStream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBaseNetView_CloseStream_Proxy( 
    IBaseNetView __RPC_FAR * This,
    /* [in] */ long hHandle);


void __RPC_STUB IBaseNetView_CloseStream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBaseNetView_InputData_Proxy( 
    IBaseNetView __RPC_FAR * This,
    /* [in] */ long hHandle,
    /* [in] */ long __RPC_FAR *pBuf,
    /* [in] */ long lSize);


void __RPC_STUB IBaseNetView_InputData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBaseNetView_OpenAudio_Proxy( 
    IBaseNetView __RPC_FAR * This,
    long __RPC_FAR *phHandle,
    long wavein,
    long waveout);


void __RPC_STUB IBaseNetView_OpenAudio_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBaseNetView_CloseAudio_Proxy( 
    IBaseNetView __RPC_FAR * This,
    long hHandle);


void __RPC_STUB IBaseNetView_CloseAudio_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IBaseNetView_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif

#include <stdlib.h>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <fpdfview.h>

#if defined(_WIN32)
#define PDFRX_EXPORT __declspec(dllexport)
#define PDFRX_INTEROP_API __stdcall
#else
#define PDFRX_EXPORT __attribute__((visibility("default"))) __attribute__((used))
#define PDFRX_INTEROP_API
#endif


struct pdfrx_file_access
{
  FPDF_FILEACCESS fileAccess;
  int retValue;
  pdfrx_read_function readBlock;
  void *param;
  std::mutex mutex;
  std::condition_variable cond;
};

static int PDFRX_INTEROP_API read(void *param,
                            unsigned long position,
                            unsigned char *pBuf,
                            unsigned long size)
{
  auto fileAccess = reinterpret_cast<pdfrx_file_access *>(param);
  std::unique_lock<std::mutex> lock(fileAccess->mutex);
  fileAccess->readBlock(fileAccess->param, position, pBuf, size);
  fileAccess->cond.wait(lock);
  return fileAccess->retValue;
}

extern "C" PDFRX_EXPORT pdfrx_file_access *PDFRX_INTEROP_API pdfrx_file_access_create(unsigned long fileSize, pdfrx_read_function readBlock, void *param)
{
  auto fileAccess = new pdfrx_file_access();
  fileAccess->fileAccess.m_FileLen = fileSize;
  fileAccess->fileAccess.m_GetBlock = read;
  fileAccess->fileAccess.m_Param = fileAccess;
  fileAccess->retValue = 0;
  fileAccess->readBlock = readBlock;
  fileAccess->param = param;
  return fileAccess;
}

extern "C" PDFRX_EXPORT void PDFRX_INTEROP_API pdfrx_file_access_destroy(pdfrx_file_access *fileAccess)
{
  delete fileAccess;
}

extern "C" PDFRX_EXPORT void PDFRX_INTEROP_API pdfrx_file_access_set_value(pdfrx_file_access *fileAccess, int retValue)
{
  std::unique_lock<std::mutex> lock(fileAccess->mutex);
  fileAccess->retValue = retValue;
  fileAccess->cond.notify_one();
}

#if defined(__APPLE__)
#include <fpdf_doc.h>
#include <fpdf_annot.h>
#include <fpdf_text.h>
#include <fpdf_edit.h>
#include <fpdf_formfill.h>

extern "C" PDFRX_EXPORT void const *const *PDFRX_INTEROP_API pdfrx_binding()
{
  static const void *bindings[] = {
      reinterpret_cast<void *>(FPDF_InitLibraryWithConfig),
      reinterpret_cast<void *>(FPDF_InitLibrary),
      reinterpret_cast<void *>(FPDF_DestroyLibrary),
      reinterpret_cast<void *>(FPDF_SetSandBoxPolicy),
      // reinterpret_cast<void*>(FPDF_SetPrintMode),
      reinterpret_cast<void *>(FPDF_LoadDocument),
      reinterpret_cast<void *>(FPDF_LoadMemDocument),
      reinterpret_cast<void *>(FPDF_LoadMemDocument64),
      reinterpret_cast<void *>(FPDF_LoadCustomDocument),
      reinterpret_cast<void *>(FPDF_GetFileVersion),
      reinterpret_cast<void *>(FPDF_GetLastError),
      reinterpret_cast<void *>(FPDF_DocumentHasValidCrossReferenceTable),
      reinterpret_cast<void *>(FPDF_GetTrailerEnds),
      reinterpret_cast<void *>(FPDF_GetDocPermissions),
      // reinterpret_cast<void*>(FPDF_GetDocUserPermissions),
      reinterpret_cast<void *>(FPDF_GetSecurityHandlerRevision),
      reinterpret_cast<void *>(FPDF_GetPageCount),
      reinterpret_cast<void *>(FPDF_LoadPage),
      reinterpret_cast<void *>(FPDF_GetPageWidthF),
      reinterpret_cast<void *>(FPDF_GetPageWidth),
      reinterpret_cast<void *>(FPDF_GetPageHeightF),
      reinterpret_cast<void *>(FPDF_GetPageHeight),
      reinterpret_cast<void *>(FPDF_GetPageBoundingBox),
      reinterpret_cast<void *>(FPDF_GetPageSizeByIndexF),
      reinterpret_cast<void *>(FPDF_GetPageSizeByIndex),
      // reinterpret_cast<void*>(FPDF_RenderPage),
      reinterpret_cast<void *>(FPDF_RenderPageBitmap),
      reinterpret_cast<void *>(FPDF_RenderPageBitmapWithMatrix),
      reinterpret_cast<void *>(FPDF_ClosePage),
      reinterpret_cast<void *>(FPDF_CloseDocument),
      reinterpret_cast<void *>(FPDF_DeviceToPage),
      reinterpret_cast<void *>(FPDF_PageToDevice),
      reinterpret_cast<void *>(FPDFBitmap_Create),
      reinterpret_cast<void *>(FPDFBitmap_CreateEx),
      reinterpret_cast<void *>(FPDFBitmap_GetFormat),
      reinterpret_cast<void *>(FPDFBitmap_FillRect),
      reinterpret_cast<void *>(FPDFBitmap_GetBuffer),
      reinterpret_cast<void *>(FPDFBitmap_GetWidth),
      reinterpret_cast<void *>(FPDFBitmap_GetHeight),
      reinterpret_cast<void *>(FPDFBitmap_GetStride),
      reinterpret_cast<void *>(FPDFBitmap_Destroy),
      reinterpret_cast<void *>(FPDF_VIEWERREF_GetPrintScaling),
      reinterpret_cast<void *>(FPDF_VIEWERREF_GetNumCopies),
      reinterpret_cast<void *>(FPDF_VIEWERREF_GetPrintPageRange),
      reinterpret_cast<void *>(FPDF_VIEWERREF_GetPrintPageRangeCount),
      reinterpret_cast<void *>(FPDF_VIEWERREF_GetPrintPageRangeElement),
      reinterpret_cast<void *>(FPDF_VIEWERREF_GetDuplex),
      reinterpret_cast<void *>(FPDF_VIEWERREF_GetName),
      reinterpret_cast<void *>(FPDF_CountNamedDests),
      reinterpret_cast<void *>(FPDF_GetNamedDestByName),
      reinterpret_cast<void *>(FPDF_GetNamedDest),
      reinterpret_cast<void *>(FPDF_GetXFAPacketCount),
      reinterpret_cast<void *>(FPDF_GetXFAPacketName),
      reinterpret_cast<void *>(FPDF_GetXFAPacketContent),
      reinterpret_cast<void *>(FPDFAnnot_IsSupportedSubtype),
      reinterpret_cast<void *>(FPDFPage_CreateAnnot),
      reinterpret_cast<void *>(FPDFPage_GetAnnotCount),
      reinterpret_cast<void *>(FPDFPage_GetAnnot),
      reinterpret_cast<void *>(FPDFPage_GetAnnotIndex),
      reinterpret_cast<void *>(FPDFPage_CloseAnnot),
      reinterpret_cast<void *>(FPDFPage_RemoveAnnot),
      reinterpret_cast<void *>(FPDFAnnot_GetSubtype),
      reinterpret_cast<void *>(FPDFAnnot_IsObjectSupportedSubtype),
      reinterpret_cast<void *>(FPDFAnnot_UpdateObject),
      reinterpret_cast<void *>(FPDFAnnot_AddInkStroke),
      reinterpret_cast<void *>(FPDFAnnot_RemoveInkList),
      reinterpret_cast<void *>(FPDFAnnot_AppendObject),
      reinterpret_cast<void *>(FPDFAnnot_GetObjectCount),
      reinterpret_cast<void *>(FPDFAnnot_GetObject),
      reinterpret_cast<void *>(FPDFAnnot_RemoveObject),
      reinterpret_cast<void *>(FPDFAnnot_SetColor),
      reinterpret_cast<void *>(FPDFAnnot_GetColor),
      reinterpret_cast<void *>(FPDFAnnot_HasAttachmentPoints),
      reinterpret_cast<void *>(FPDFAnnot_SetAttachmentPoints),
      reinterpret_cast<void *>(FPDFAnnot_AppendAttachmentPoints),
      reinterpret_cast<void *>(FPDFAnnot_CountAttachmentPoints),
      reinterpret_cast<void *>(FPDFAnnot_GetAttachmentPoints),
      reinterpret_cast<void *>(FPDFAnnot_SetRect),
      reinterpret_cast<void *>(FPDFAnnot_GetRect),
      reinterpret_cast<void *>(FPDFAnnot_GetVertices),
      reinterpret_cast<void *>(FPDFAnnot_GetInkListCount),
      reinterpret_cast<void *>(FPDFAnnot_GetInkListPath),
      reinterpret_cast<void *>(FPDFAnnot_GetLine),
      reinterpret_cast<void *>(FPDFAnnot_SetBorder),
      reinterpret_cast<void *>(FPDFAnnot_GetBorder),
      reinterpret_cast<void *>(FPDFAnnot_GetFormAdditionalActionJavaScript),
      reinterpret_cast<void *>(FPDFAnnot_HasKey),
      reinterpret_cast<void *>(FPDFAnnot_GetValueType),
      reinterpret_cast<void *>(FPDFAnnot_SetStringValue),
      reinterpret_cast<void *>(FPDFAnnot_GetStringValue),
      reinterpret_cast<void *>(FPDFAnnot_GetNumberValue),
      reinterpret_cast<void *>(FPDFAnnot_SetAP),
      reinterpret_cast<void *>(FPDFAnnot_GetAP),
      reinterpret_cast<void *>(FPDFAnnot_GetLinkedAnnot),
      reinterpret_cast<void *>(FPDFAnnot_GetFlags),
      reinterpret_cast<void *>(FPDFAnnot_SetFlags),
      reinterpret_cast<void *>(FPDFAnnot_GetFormFieldFlags),
      reinterpret_cast<void *>(FPDFAnnot_GetFormFieldAtPoint),
      reinterpret_cast<void *>(FPDFAnnot_GetFormFieldName),
      reinterpret_cast<void *>(FPDFAnnot_GetFormFieldAlternateName),
      reinterpret_cast<void *>(FPDFAnnot_GetFormFieldType),
      reinterpret_cast<void *>(FPDFAnnot_GetFormFieldValue),
      reinterpret_cast<void *>(FPDFAnnot_GetOptionCount),
      reinterpret_cast<void *>(FPDFAnnot_GetOptionLabel),
      reinterpret_cast<void *>(FPDFAnnot_IsOptionSelected),
      reinterpret_cast<void *>(FPDFAnnot_GetFontSize),
      reinterpret_cast<void *>(FPDFAnnot_IsChecked),
      reinterpret_cast<void *>(FPDFAnnot_SetFocusableSubtypes),
      reinterpret_cast<void *>(FPDFAnnot_GetFocusableSubtypesCount),
      reinterpret_cast<void *>(FPDFAnnot_GetFocusableSubtypes),
      reinterpret_cast<void *>(FPDFAnnot_GetLink),
      reinterpret_cast<void *>(FPDFAnnot_GetFormControlCount),
      reinterpret_cast<void *>(FPDFAnnot_GetFormControlIndex),
      reinterpret_cast<void *>(FPDFAnnot_GetFormFieldExportValue),
      reinterpret_cast<void *>(FPDFAnnot_SetURI),
      reinterpret_cast<void *>(FPDFText_LoadPage),
      reinterpret_cast<void *>(FPDFText_ClosePage),
      reinterpret_cast<void *>(FPDFText_CountChars),
      reinterpret_cast<void *>(FPDFText_GetUnicode),
      reinterpret_cast<void *>(FPDFText_IsGenerated),
      reinterpret_cast<void *>(FPDFText_IsHyphen),
      reinterpret_cast<void *>(FPDFText_HasUnicodeMapError),
      reinterpret_cast<void *>(FPDFText_GetFontSize),
      reinterpret_cast<void *>(FPDFText_GetFontInfo),
      reinterpret_cast<void *>(FPDFText_GetFontWeight),
      // reinterpret_cast<void *>(FPDFText_GetTextRenderMode),
      reinterpret_cast<void *>(FPDFText_GetFillColor),
      reinterpret_cast<void *>(FPDFText_GetStrokeColor),
      reinterpret_cast<void *>(FPDFText_GetCharAngle),
      reinterpret_cast<void *>(FPDFText_GetCharBox),
      reinterpret_cast<void *>(FPDFText_GetLooseCharBox),
      reinterpret_cast<void *>(FPDFText_GetMatrix),
      reinterpret_cast<void *>(FPDFText_GetCharOrigin),
      reinterpret_cast<void *>(FPDFText_GetCharIndexAtPos),
      reinterpret_cast<void *>(FPDFText_GetText),
      reinterpret_cast<void *>(FPDFText_CountRects),
      reinterpret_cast<void *>(FPDFText_GetRect),
      reinterpret_cast<void *>(FPDFText_GetBoundedText),
      reinterpret_cast<void *>(FPDFText_FindStart),
      reinterpret_cast<void *>(FPDFText_FindNext),
      reinterpret_cast<void *>(FPDFText_FindPrev),
      reinterpret_cast<void *>(FPDFText_GetSchResultIndex),
      reinterpret_cast<void *>(FPDFText_GetSchCount),
      reinterpret_cast<void *>(FPDFText_FindClose),
      reinterpret_cast<void *>(FPDFLink_LoadWebLinks),
      reinterpret_cast<void *>(FPDFLink_CountWebLinks),
      reinterpret_cast<void *>(FPDFLink_LoadWebLinks),
      reinterpret_cast<void *>(FPDFLink_CountWebLinks),
      reinterpret_cast<void *>(FPDFLink_GetURL),
      reinterpret_cast<void *>(FPDFLink_CountRects),
      reinterpret_cast<void *>(FPDFLink_GetRect),
      reinterpret_cast<void *>(FPDFLink_GetTextRange),
      reinterpret_cast<void *>(FPDFLink_CloseWebLinks),
      reinterpret_cast<void *>(FPDFLink_GetLinkAtPoint),
      reinterpret_cast<void *>(FPDFLink_GetLinkZOrderAtPoint),
      reinterpret_cast<void *>(FPDFLink_GetDest),
      reinterpret_cast<void *>(FPDFLink_GetAction),
      reinterpret_cast<void *>(FPDFLink_Enumerate),
      reinterpret_cast<void *>(FPDFLink_GetAnnot),
      reinterpret_cast<void *>(FPDFLink_GetAnnotRect),
      reinterpret_cast<void *>(FPDFLink_CountQuadPoints),
      reinterpret_cast<void *>(FPDFLink_GetQuadPoints),
      reinterpret_cast<void *>(FPDFLink_CloseWebLinks),
      reinterpret_cast<void *>(FPDFAction_GetType),
      reinterpret_cast<void *>(FPDFAction_GetDest),
      reinterpret_cast<void *>(FPDFAction_GetFilePath),
      reinterpret_cast<void *>(FPDFAction_GetURIPath),
      reinterpret_cast<void *>(FPDFDest_GetDestPageIndex),
      reinterpret_cast<void *>(FPDFDest_GetView),
      reinterpret_cast<void *>(FPDFDest_GetLocationInPage),
      reinterpret_cast<void *>(FPDFBookmark_GetFirstChild),
      reinterpret_cast<void *>(FPDFBookmark_GetNextSibling),
      reinterpret_cast<void *>(FPDFBookmark_GetTitle),
      reinterpret_cast<void *>(FPDFBookmark_GetCount),
      reinterpret_cast<void *>(FPDFBookmark_Find),
      reinterpret_cast<void *>(FPDFBookmark_GetDest),
      reinterpret_cast<void *>(FPDFBookmark_GetAction),
      reinterpret_cast<void *>(FPDFDOC_InitFormFillEnvironment),
      reinterpret_cast<void *>(FPDFDOC_ExitFormFillEnvironment),
      reinterpret_cast<void *>(FPDF_FFLDraw),
      reinterpret_cast<void *>(FPDF_GetFormType),
      reinterpret_cast<void *>(FPDF_CreateNewDocument),
      reinterpret_cast<void *>(FPDFPage_New),
      reinterpret_cast<void *>(FPDFPage_Delete),
      reinterpret_cast<void *>(FPDF_MovePages),
      reinterpret_cast<void *>(FPDFPage_GetRotation),
      reinterpret_cast<void *>(FPDFPage_SetRotation),
      reinterpret_cast<void *>(FPDFPage_InsertObject),
      reinterpret_cast<void *>(FPDFPage_RemoveObject),
      reinterpret_cast<void *>(FPDFPage_CountObjects),
      reinterpret_cast<void *>(FPDFPage_GetObject),
      reinterpret_cast<void *>(FPDFPage_HasTransparency),
      reinterpret_cast<void *>(FPDFPage_GenerateContent),
      reinterpret_cast<void *>(FPDFPageObj_Destroy),
      reinterpret_cast<void *>(FPDFPageObj_HasTransparency),
      reinterpret_cast<void *>(FPDFPageObj_GetType),
      reinterpret_cast<void *>(FPDFPageObj_Transform),
      reinterpret_cast<void *>(FPDFPageObj_GetMatrix),
      reinterpret_cast<void *>(FPDFPageObj_SetMatrix),
      reinterpret_cast<void *>(FPDFPage_TransformAnnots),
      reinterpret_cast<void *>(FPDFPageObj_NewImageObj),
      reinterpret_cast<void *>(FPDFPageObj_CountMarks),
      reinterpret_cast<void *>(FPDFPageObj_GetMark),
      reinterpret_cast<void *>(FPDFPageObj_AddMark),
      reinterpret_cast<void *>(FPDFPageObj_RemoveMark),
      reinterpret_cast<void *>(FPDFPageObjMark_GetName),
      reinterpret_cast<void *>(FPDFPageObjMark_CountParams),
      reinterpret_cast<void *>(FPDFPageObjMark_GetParamKey),
      reinterpret_cast<void *>(FPDFPageObjMark_GetParamValueType),
      reinterpret_cast<void *>(FPDFPageObjMark_GetParamIntValue),
      reinterpret_cast<void *>(FPDFPageObjMark_GetParamStringValue),
      reinterpret_cast<void *>(FPDFPageObjMark_GetParamBlobValue),
      reinterpret_cast<void *>(FPDFPageObjMark_SetIntParam),
      reinterpret_cast<void *>(FPDFPageObjMark_SetStringParam),
      reinterpret_cast<void *>(FPDFPageObjMark_SetBlobParam),
      reinterpret_cast<void *>(FPDFPageObjMark_RemoveParam),
      reinterpret_cast<void *>(FPDFImageObj_LoadJpegFile),
      reinterpret_cast<void *>(FPDFImageObj_LoadJpegFileInline),
      reinterpret_cast<void *>(FPDFImageObj_SetMatrix),
      reinterpret_cast<void *>(FPDFImageObj_SetBitmap),
      reinterpret_cast<void *>(FPDFImageObj_GetBitmap),
      reinterpret_cast<void *>(FPDFImageObj_GetRenderedBitmap),
      reinterpret_cast<void *>(FPDFImageObj_GetImageDataDecoded),
      reinterpret_cast<void *>(FPDFImageObj_GetImageDataRaw),
      reinterpret_cast<void *>(FPDFImageObj_GetImageFilterCount),
      reinterpret_cast<void *>(FPDFImageObj_GetImageFilter),
      reinterpret_cast<void *>(FPDFImageObj_GetImageMetadata),
      reinterpret_cast<void *>(FPDFImageObj_GetImagePixelSize),
      reinterpret_cast<void *>(FPDFPageObj_CreateNewPath),
      reinterpret_cast<void *>(FPDFPageObj_CreateNewRect),
      reinterpret_cast<void *>(FPDFPageObj_GetBounds),
      reinterpret_cast<void *>(FPDFPageObj_GetRotatedBounds),
      reinterpret_cast<void *>(FPDFPageObj_SetBlendMode),
      reinterpret_cast<void *>(FPDFPageObj_SetStrokeColor),
      reinterpret_cast<void *>(FPDFPageObj_GetStrokeColor),
      reinterpret_cast<void *>(FPDFPageObj_SetStrokeWidth),
      reinterpret_cast<void *>(FPDFPageObj_GetStrokeWidth),
      reinterpret_cast<void *>(FPDFPageObj_GetLineJoin),
      reinterpret_cast<void *>(FPDFPageObj_SetLineJoin),
      reinterpret_cast<void *>(FPDFPageObj_GetLineCap),
      reinterpret_cast<void *>(FPDFPageObj_SetLineCap),
      reinterpret_cast<void *>(FPDFPageObj_SetFillColor),
      reinterpret_cast<void *>(FPDFPageObj_GetFillColor),
      reinterpret_cast<void *>(FPDFPageObj_GetDashPhase),
      reinterpret_cast<void *>(FPDFPageObj_SetDashPhase),
      reinterpret_cast<void *>(FPDFPageObj_GetDashCount),
      reinterpret_cast<void *>(FPDFPageObj_GetDashArray),
      reinterpret_cast<void *>(FPDFPageObj_SetDashArray),
      reinterpret_cast<void *>(FPDFPath_CountSegments),
      reinterpret_cast<void *>(FPDFPath_GetPathSegment),
      reinterpret_cast<void *>(FPDFPathSegment_GetPoint),
      reinterpret_cast<void *>(FPDFPathSegment_GetType),
      reinterpret_cast<void *>(FPDFPathSegment_GetClose),
      reinterpret_cast<void *>(FPDFPath_MoveTo),
      reinterpret_cast<void *>(FPDFPath_LineTo),
      reinterpret_cast<void *>(FPDFPath_BezierTo),
      reinterpret_cast<void *>(FPDFPath_Close),
      reinterpret_cast<void *>(FPDFPath_SetDrawMode),
      reinterpret_cast<void *>(FPDFPath_GetDrawMode),
      reinterpret_cast<void *>(FPDFPageObj_NewTextObj),
      reinterpret_cast<void *>(FPDFText_SetText),
      reinterpret_cast<void *>(FPDFText_SetCharcodes),
      reinterpret_cast<void *>(FPDFText_LoadFont),
      reinterpret_cast<void *>(FPDFText_LoadStandardFont),
      reinterpret_cast<void *>(FPDFTextObj_GetFontSize),
      reinterpret_cast<void *>(FPDFFont_Close),
      reinterpret_cast<void *>(FPDFPageObj_CreateTextObj),
      reinterpret_cast<void *>(FPDFTextObj_GetTextRenderMode),
      reinterpret_cast<void *>(FPDFTextObj_SetTextRenderMode),
      reinterpret_cast<void *>(FPDFTextObj_GetText),
      reinterpret_cast<void *>(FPDFTextObj_GetRenderedBitmap),
      reinterpret_cast<void *>(FPDFTextObj_GetFont),
      // reinterpret_cast<void *>(FPDFFont_GetFontName),
      reinterpret_cast<void *>(FPDFFont_GetFontData),
      reinterpret_cast<void *>(FPDFFont_GetIsEmbedded),
      reinterpret_cast<void *>(FPDFFont_GetFlags),
      reinterpret_cast<void *>(FPDFFont_GetWeight),
      reinterpret_cast<void *>(FPDFFont_GetItalicAngle),
      reinterpret_cast<void *>(FPDFFont_GetAscent),
      reinterpret_cast<void *>(FPDFFont_GetDescent),
      reinterpret_cast<void *>(FPDFFont_GetGlyphWidth),
      reinterpret_cast<void *>(FPDFFont_GetGlyphPath),
      reinterpret_cast<void *>(FPDFGlyphPath_CountGlyphSegments),
      reinterpret_cast<void *>(FPDFGlyphPath_GetGlyphPathSegment),
      reinterpret_cast<void *>(FPDFFormObj_CountObjects),
      reinterpret_cast<void *>(FPDFFormObj_GetObject),
  };
  return bindings;
}
#endif

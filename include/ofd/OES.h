#ifndef __OFD_OES_H__
#define __OFD_OES_H__

namespace ofd{

#define OES_OK 0x00000000
#define OES_CANCEL 0x00000010

#define OES_SEALIMAGE_FLAG_DISPLAY 0x00000000
#define OES_SEALIMAGE_FLAG_PRINT 0x00000001
#define OES_SEALIMAGE_FLAG_PREVIEW 0x00000002

// 返回签章组件提供者信息
int OES_GetProviderInfo(
        // 名称（UTF-8编码）
        unsigned char *puchName, int *piNameLen,
        // 公司名称（UTF-8编码）
        unsigned char *puchCompany, int *piCompanyLen,
        // 版本（UTF-8编码）
        unsigned char *puchVersion, int *piVersionLen,
        // 扩展信息（UTF-8编码）
        unsigned char *puchExtend, int *piExtendLen);


// 获取电子印章列表
int OES_GetSealList(
        // 印章列表数据
        unsigned char *puchSealListData, int *piSealListDataLen);

// 获取电子印章
int OES_GetSeal(
        // 印章标识或名称
        unsigned char *puchSealId, int iSealIdLen,
        // 印章数据
        unsigned char *puchSealData, int *piSealDataLen);

// 获取电子印章信息
int OES_GetSealInfo(
        // 印章数据
        unsigned char *puchSealData, int iSealDataLen,
        // 头信息-印章标识
        unsigned char *puchSealId, int *piSealIdLen,
        // 头信息-版本
        unsigned char *puchVersion, int *piVersionLen,
        // 头信息-厂商标识
        unsigned char *puchVenderId, int *piVenderIdLen,
        // 印章信息-印章类型
        unsigned char *puchSealType, int *piSealTypeLen,
        // 印章信息-印章名称 
        unsigned char *puchSealName, int *piSealNameLen,
        // 印章信息-证书列表信息
        unsigned char *puchCertInfo, int *piCertInfoLen,
        // 印章信息-有效起始时间
        unsigned char *puchValidStart, int *piValidStartLen,
        // 印章信息-有效结束时间
        unsigned char *puchValidEnd, int *piValidEndLen,
        // 印章信息-制作时间
        unsigned char *puchSignedDate, int *piSignedDateLen,
        // 签名信息-制章人
        unsigned char *puchSignerName, int *piSignerNameLen,
        // 签名信息-制章签名方法
        unsigned char *puchSignMethod, int *piSignMethodLen);

// 获取电子印章图像
int OES_GetSealImage(
        // 印章数据
        unsigned char *puchSealData, int iSealDataLen,
        // 绘制用途标记
        int iRenderFlag,
        // 印章图像数据
        unsigned char *puchSealImage, int *piSealImageLen,
        // 印章宽度
        int *piSealWidth,
        // 印章高度
        int *piSealHeight);

// 获取签名时间
int OES_GetSignDateTime(
        // 签名时间（字符时用UTF-8编码，时间戳时二进制值）
        unsigned char *puchSignDateTime, int *piSignDateTimeLen);

// 获取签名算法标识
int OES_GetSignMethod(
        // 签名算法
        unsigned char *puchSignMethod, int *piSignMethodLen);

// 获取摘要算法标识
int OES_GetDigestMethod(
        // 摘要算法
        unsigned char *puchDigestMethod, int *piDigestMethodLen);

// 代理计算摘要
int OES_Digest(
        // 待摘要的数据
        unsigned char *puchData, int iDateLen,
        // 摘要算法
        unsigned char *puchDigestMethod, int iDigestMethodLen,
        // 摘要值
        unsigned char *puchDigestValue, int *piDigestValueLen);

// 代理计算签名
int OES_Sign(
        // 印章标识
        unsigned char *puchSealId, int iSealIdLen,
        // 文档信息
        unsigned char *puchDocProperty, int iDocPropertyLen,
        // 摘要数据
        unsigned char *puchDigestData, int iDigestDataLen,
        // 签名算法
        unsigned char *puchSignMethod, int iSignMethodLen,
        // 签名时间
        unsigned char *puchSignDateTime, int iSignDateTimeLen,
        // 签名值
        unsigned char *puchSignValue, int *piSignValueLen);

// 代理验证签名
int OES_Verify(
        // 印章数据
        unsigned char *puchSealData, int iSealDataLen,
        // 文档信息
        unsigned char *puchDocProperty, int iDocPropertyLen,
        // 签名算法
        unsigned char *puchSignMethod, int iSignMethodLen,
        // 签名时间
        unsigned char *puchSignDateTime, int iSignDateTimeLen,
        // 签名值
        unsigned char *puchSignValue, int iSignValueLen,
        // 是否在线验证
        int iOnline);

// 获取电子签章图像
int OES_GetSignImage(
        // 签章数据
        unsigned char *puchSignedValueData, int iSignedValueLen,
        // 绘制用途标记
        int iRenderFlag,
        // 印章图像数据
        unsigned char *puchSealImage, int *piSealImageLen,
        // 印章宽度（单位mm）
        int *piSealWidth,
        // 印章高度（单位mm）
        int *piSealHeight);

// 获取错误信息
int OES_GetErrMessage(
        // 错误代码
        unsigned long errCode, 
        // 错误信息
        unsigned char *puchErrMessage, int *piErrMessageLen);

}; // namespace ofd

#endif // __OFD_OES_H__

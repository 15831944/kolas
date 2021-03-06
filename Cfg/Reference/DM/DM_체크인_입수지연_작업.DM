#[Data Manager]62432627 ▶ DM_체크인_입수지연_작업
ALIAS=DM_체크인_입수지연_작업
CONNECTION_INFO=kolas2@k2up/kolas2#[]
TBL_NAME=IDX_SE_TBL I, SE_SPECIES_TBL S, SE_VOL_TBL V, SE_ACQ_TYPE_VOL_TBL A
CONDITION=I.MANAGE_CODE=UDF_MANAGE_CODE
EXTRA_CONDITION=
#Field Alias|Field Name|Field Type|InitValue|Data Type|
SA_입수책수|A.ACQ_BOOK_CNT|int|||
SA_수입구분|A.ACQ_CODE|str|||
SA_입수예정책수|A.ACQ_EXP_BOOK_CNT|int|||
SA_입수예정일|A.ACQ_EXP_DATE|str|||
SA_입수예측권호명|A.ACQ_EXP_VOL_TITLE|str|||
SA_수입구분KEY|A.ACQ_KEY|int|||
UDF_입수상태|A.CHECKIN_STATUS|str|||
SA_입수상태|A.CHECKIN_STATUS|str|||
SA_최초작업|A.FIRST_WORK|str|||
SA_마지막작업|A.LAST_WORK|str|||
SA_결호책수|A.MISS_BOOK_CNT|int|||
SA_입수처|A.PROVIDER|str|||
SA_수입구분별권KEY|A.REC_KEY|int|||
SA_권KEY|A.VOL_KEY|int|||
ISS_제어번호|I.CONTROL_NO|str|||
ISS_형식구분_코드|I.FORM_CODE|str|||
SS_관리구분_코드|S.MANAGE_CODE|str|||
ISS_매체구분_코드|I.MEDIA_CODE|str|||
ISS_종KEY|I.REC_KEY|int|||
ISS_등록구분_코드|I.REG_CODE|str|||
SS_이용제한구분_코드|S.USE_LIMIT_CODE|str|||
SS_제본여부|S.BINDING_YN|str|||
SS_분류기호구분_코드|S.CLASS_NO_TYPE|str|||
SS_화폐구분_코드|S.CURRENCY_CODE|str|||
SS_가격|S.PRICE|int|||
SS_가격성격|S.PRICE_CHARACTER|str|||
SS_등록구분_코드|S.REG_CODE|str|||
SS_배가위치_코드|S.SHELF_LOC_CODE|str|||
SV_딸림자료|V.ACCOMP_MAT|str|||
SV_권호별서명|V.ALTERNATIVE_VOL_TITLE|str|||
SV_책크기|V.BOOK_SIZE|str|||
SV_목차정보KEY|V.CONTENTS_KEY|int|||
SV_목차유무|V.CONTENTS_YN|str|||
SV_화폐구분|V.CURRENCY_CODE|str|||
SV_최초작업|V.FIRST_WORK|str|||
SV_형식구분_코드|V.FORM_CODE|str|||
SV_색인권호명|V.IDX_VOL_TITLE|str|||
SV_마지막작업|V.LAST_WORK|str|||
SV_매체구분_코드|V.MEDIA_CODE|str|||
SV_부차적권호명|V.OTHER_VOL_TITLE|str|||
SV_면장수|V.PAGE|str|||
SV_물리적특성|V.PHYSICAL_PROPERTY|str|||
SV_가격|V.PRICE|int|||
SV_가격성격|V.PRICE_CHARACTER|str|||
SV_발행일|V.PUBLISH_DATE|str|||
SV_권KEY|V.REC_KEY|int|||
SV_비고|V.REMARK|str|||
SV_종KEY|V.SPECIES_KEY|int|||
SV_권호표제|V.TITLE|str|||
SV_권호구분|V.VOLUME_CLASS|str|||
UDF_권호구분|V.VOLUME_CLASS|str|||
SV_권호일련번호|V.VOL_SORT_NO|int|||
SV_권호명|V.VOL_TITLE|str|||
SV_권호유형|V.VOL_TYPE|str|||
UDF_상태||||UDF|
UDF_다음발행예정일||||UDF|
UDF_부차적권호예측시작수||||UDF|
UDF_발행예측된갯수||||UDF|
UDF_등록책수||||UDF|
UDF_열람우선여부||||UDF|
UDF_입수일||||UDF|
UDF_입수예측된갯수||||UDF|
UDF_권호예측시작수||||UDF|
UDF_다음입수예정일||||UDF|
UDF_행||||UDF|
UDF_기사수||||UDF|
UDF_부록수||||UDF|
UDF_독촉차수||||UDF|
UDF_독촉일||||UDF|
UDF_지연수||||UDF|

# TEST TRACKING

## 2019-06-17

### modis_archive.exe

- [ ] Add a space before "...has been archived.."
- [ ] send code to Dr.Jiao

### proc_MxD04_3k.exe

- [ ] tmp folder no deleted
- [ ] heg cmd window present
- [ ] Why so many negative values?
- [ ] projection name should not be WGS_84
- [ ] Changing *HdfFilePath* to *HDFListFile*
- [ ] Changing *TempDir* to *TmpPath*

### proc_MxD05_L2.exe

- [ ] the valid of water vapor is 0~20 cm, many abnormal value presented
- [ ] heg cmd window present, as proc_MxD04_3k.exe
- [ ] Changing *HdfFilePath* to *HDFListFile*
- [ ] Changing *TempDir* to *TmpPath*
- [ ] tmp folder no clear

### proc_MxD11A1.exe

- [ ] Changing *InputHdfFile* to *HDFListFile*
- [ ] drop parameter *INPUT_LAT_LONG*
- [ ] drop parameter *Datum: NoDatum*
- [ ] Changing *TempDir* to *TmpPath*
- [ ] tmp folder no clear
- [ ] data is illegal, valid data generally under 320K
- [ ] No Datum information, generally WGS84 [in map info] 

### proc_MxD021km.exe

- [ ] put Aqua_MODIS_rad2BT_LUT.txt and Terra_MODIS_rad2BT_LUT.txt  to /data folder
- [ ] Changing *InputHdfFile* to *HDFListFile*
- [ ] Changing *TempDir* to *TmpPath*
- [ ] Hide matrix information on log info level

### eddy_filed.exe

- [ ] Changing *ModisWorkspaceFolder* to *Workspace*
- [ ] Changing *NeedComputeRef* to *CalcRef*
- [ ] Changing *NeedComputeEddyField* to *CalcAno*
- [ ] Changing *Method* to *AnoMethod*
- [ ] Changing *OutputEddyFieldImageFile* to *OutputAnoFile*
- [ ] Changing *RefTifFile* to *RefListFile*
- [ ] Changing *eddy_field.exe* to *eddy_field_ano.exe*
- [ ] Sample data sending
- [ ] Send code

### general_ano.exe

- [ ] Changing *general_ano.exe* to *juping_ano.exe*
- [ ] Changing *TempDir* to *TmpPath*
- [ ] Changing *InputMonthTifFile* to *MonthListFile*
- [ ] Changing *InputRefTifFile* to *RefListFile*
- [ ] Sending code

### rst_en.exe

- [ ] Changing *rst_en.exe* to *rst_en_ano.exe*
- [ ] .yml file is not well organised, we will have a talk
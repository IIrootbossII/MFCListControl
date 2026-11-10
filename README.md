
# MFC CSV List Control

C++과 MFC를 이용해 CSV 파일의 데이터를 표 형태로 표시하고, 셀 단위로 수정한 뒤 다시 CSV 파일로 저장할 수 있도록 제작한 Windows 데스크톱 애플리케이션입니다.

`CListCtrl`의 OwnerData(가상 리스트) 방식과 `std::vector` 기반 데이터 관리를 학습하고, 파일 입출력·이벤트 처리·GUI 간의 연결을 구현하는 데 중점을 두었습니다.

파일 내 MemberList.CSV 예제 파일을 다운로드하시기 바랍니다.


## 1. 개발 환경 (Development Environment)

| 구분 | 사용 기술 |
| --- | --- |
| 언어 | C++ |
| 프레임워크 | MFC (Microsoft Foundation Classes) |
| 개발 도구 | Microsoft Visual Studio |
| 주요 클래스 | `CDialogEx`, `CListCtrl`, `CEdit`, `CStdioFile`, `CFileDialog`, `CFontDialog`, `CFont` |
| 데이터 관리 | `Person`, `std::vector<Person>`, `CString` |



## 2. 주요 기능 (Features)

- **CSV 열기(Open):** 파일 선택 창을 통해 CSV를 선택하고, 파일의 데이터를 읽어 표에 표시합니다.
- **데이터 표시:** `CListCtrl`을 Report 형식으로 사용하고, `LVN_GETDISPINFO`에서 요청된 셀의 문자열을 제공합니다.
- **셀 선택 및 수정(Edit):** 클릭한 셀의 행·열 위치를 파악해 편집창에 값을 표시하고, 수정값을 메모리의 데이터에 반영합니다.
- **CSV 저장(Save):** 메모리에 저장된 데이터를 CSV 파일로 출력합니다.
- **목록 초기화(Close):** 데이터와 리스트의 열을 초기화합니다.
- **폰트 설정(Font):** 글꼴 선택 대화상자를 통해 리스트와 편집창의 폰트를 변경합니다.

> 현재 소스의 CSV 파싱·저장 처리에는 수정이 필요한 부분이 있습니다. 일반적인 모든 CSV 형식을 완전하게 지원하는 구현은 아닙니다.

---

## 3. 주요 구현 내용 (Implementation)

### 3.1 행 단위 데이터 모델

CSV의 한 행을 `Person` 클래스로 표현하고, 전체 데이터를 `std::vector<Person>`에 저장하도록 구성했습니다.

`Person::GetString(int)`은 필드 번호에 따라 문자열을 반환하며, 정수 필드는 `CString::Format()`으로 변환합니다.

### 3.2 OwnerData 방식의 리스트 표시

리스트 컨트롤의 표시용 문자열을 개별 항목에 직접 저장하는 대신, `m_vecData`에서 조회하도록 구성했습니다.

```cpp
m_listCtrl.SetItemCount((int)m_vecData.size());
```

리스트에서 `LVN_GETDISPINFO` 알림을 받으면 행·열 인덱스를 확인하고, `Person::GetString()`이 반환한 문자열을 컨트롤이 제공한 버퍼에 복사합니다.

```cpp
_tcsncpy_s(
    pDisp->item.pszText,
    pDisp->item.cchTextMax,
    (LPCTSTR)m_vecData[x].GetString(y),
    _TRUNCATE
);
```

> OwnerData 방식으로 실행하려면 리스트 컨트롤에 `LVS_OWNERDATA` 스타일이 설정되어 있어야 합니다. 리소스 설정은 이 문서 작성에 제공된 코드만으로 확인되지 않았습니다.

### 3.3 CSV 파일 읽기 및 데이터 변환

`CFileDialog`에서 파일을 선택하고 `CStdioFile::ReadString()`으로 한 줄씩 읽습니다. 필드를 분리한 후 9개 항목을 `Person` 객체에 할당합니다.

문자열 필드에는 `CString`, 숫자 필드에는 `_ttoi()`를 사용합니다.

### 3.4 선택 셀 편집

`SubItemHitTest()`로 클릭한 셀의 행·열 인덱스를 구하고, 해당 셀의 값을 `CEdit`에 표시합니다.

Edit 버튼을 누르면 열 번호에 따라 `Person`의 해당 필드를 변경한 후 `Invalidate()`로 표시 갱신을 요청합니다.

### 3.5 폰트 변경

`CFontDialog`로 글꼴을 선택하고, `CFont::CreatePointFont()`로 생성한 폰트를 `CListCtrl`과 `CEdit`에 적용합니다.



## 4. 실행 화면 (Screenshots)

프로그램의 실제 실행 화면을 추가할 예정입니다.

- CSV 파일을 불러온 화면
- 셀을 선택하고 편집창에 값을 표시한 화면
- 폰트 변경 화면



## 5. 개선 사항 및 알려진 제약 (Improvements & Limitations)

### 우선 수정할 항목

- CSV 저장 시 헤더 끝에 줄바꿈이 없어 첫 데이터 행이 이어 붙는 문제
- 필드 개수를 `arr.size() > 9`로 검사해 정확히 9개인 데이터 행을 건너뛰는 문제
- 파일을 닫은 후 선택한 행·열 인덱스가 남는 문제
- CSV 헤더의 열 개수를 확인하지 않고 9개 열을 읽는 부분
- 편집 시 행·열 인덱스의 유효성 검사가 충분하지 않은 부분

### 향후 개선 방향

- 쉼표·큰따옴표·줄바꿈이 포함된 필드를 올바르게 처리하는 CSV 파서 및 저장 로직
- CSV 파일의 문자 인코딩 처리 개선
- 잘못된 숫자 입력에 대한 유효성 검사 및 오류 안내
- 데이터 모델과 UI 로직의 추가 분리
- 파일 열기·저장 실패에 대한 오류 처리 보강



## 6. 빌드 및 실행 (Build & Run)

1. Windows에서 MFC 개발 구성 요소가 설치된 Visual Studio를 준비합니다.
2. 프로젝트의 솔루션 파일(`.sln`)을 엽니다.
3. 프로젝트를 빌드하고 실행합니다.
4. **Open** 버튼으로 9개 주요 필드를 포함하는 CSV 파일을 선택합니다.
5. 셀 선택, 편집, 폰트 변경 및 저장 기능을 사용합니다.

> 본 문서는 제공된 소스 코드를 바탕으로 작성했습니다. 실행·빌드 테스트와 CSV 파일의 다양한 형식에 대한 검증을 완료했다고 의미하지는 않습니다.

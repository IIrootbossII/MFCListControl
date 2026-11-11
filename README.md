
# MFC CSV List Control

C++과 MFC(Microsoft Foundation Classes)를 사용해 만든 Windows용 CSV 데이터 조회·편집 프로그램입니다.

CSV 파일을 불러와 `CListCtrl`에 표 형태로 표시하고, 선택한 셀의 값을 수정하거나 글꼴을 변경한 뒤 CSV 파일로 저장할 수 있도록 구현했습니다. 처음에는 `Person` 클래스로 9개 열을 고정 관리했으나, 이후 CSV 파일의 **전체 열을 처리할 수 있는 2차원 벡터 구조**로 변경했습니다.



## 1. 개발 환경 (Development Environment)

| 항목 | 내용 |
| --- | --- |
| Language | C++ |
| Framework | MFC (Microsoft Foundation Classes) |
| IDE | Microsoft Visual Studio |
| Platform | Windows |
| 주요 클래스 | `CDialogEx`, `CListCtrl`, `CEdit`, `CFileDialog`, `CStdioFile`, `CFontDialog`, `CFont`, `CString` |
| 데이터 구조 | `std::vector<CString>`, `std::vector<std::vector<CString>>` |



## 2. 주요 기능 (Features)

- **Open:** CSV 파일 선택 및 데이터 불러오기
- **List Control:** CSV의 전체 열과 행을 Report 형식으로 표시
- **Edit:** 선택한 셀의 값을 편집창에서 수정
- **Save:** 데이터 및 열 제목을 CSV 형식으로 저장
- **Close:** 현재 데이터와 리스트 초기화
- **Font:** 글꼴 및 크기 변경
- **OwnerData:** `LVN_GETDISPINFO`를 이용하여 요청받은 셀의 데이터 제공
- **열 개수 검사:** 데이터 행의 필드 개수가 헤더와 다르면 오류 메시지 출력



## 3. 주요 구현 내용 (Implementation)

### 3.1 CSV 데이터 읽기

`CFileDialog`로 파일을 선택하고, `CStdioFile::ReadString()`으로 텍스트를 순차적으로 읽습니다. `mRowString()`은 쉼표로 구분된 필드를 `std::vector<CString>`에 저장하며, 큰따옴표로 감싼 쉼표와 여러 줄에 걸친 필드도 고려하도록 구성했습니다.

첫 번째 레코드는 열 제목으로 처리하고, 나머지 레코드는 데이터 행으로 저장합니다.

### 3.2 고정 9열 구조에서 가변 열 구조로 개선

초기에는 `Person` 클래스에 9개의 필드를 선언하고 열 번호별로 각 멤버에 값을 대입했습니다. 이후 파일마다 열 개수가 달라질 수 있다는 점을 고려해 아래 구조로 변경했습니다.

```cpp
std::vector<CString> m_vecHeader{};
std::vector<std::vector<CString>> m_vecData{};
```

- `m_vecHeader`는 CSV의 열 제목을 보관합니다.
- `m_vecData`는 데이터 행을 보관하며 `m_vecData[행][열]` 형식으로 셀에 접근합니다.
- 각 행은 `m_vecData.push_back(arr)`로 추가합니다.

데이터 행의 열 개수가 헤더의 열 개수와 다르면 파일 읽기를 중단하고 오류를 표시하도록 했습니다.

```cpp
if (arr.size() != m_vecHeader.size())
{
    // 열 개수 불일치 오류 처리
    // 파일 닫기 및 데이터 초기화 후 return
}

m_vecData.push_back(arr);
```

### 3.3 OwnerData 기반 리스트 표시

`CListCtrl`에 `LVS_OWNERDATA` 스타일을 사용하고, `SetItemCount()`로 표시할 데이터 행 개수를 설정합니다. 각 셀의 문자는 리스트 컨트롤의 요청(`LVN_GETDISPINFO`)에 따라 `m_vecData`에서 찾아 전달합니다.

```cpp
m_listCtrl.SetItemCount((int)m_vecData.size());
```

이 방식에서는 리스트 컨트롤에 모든 문자열을 직접 삽입하지 않고, 실제 데이터와 화면 표시를 분리할 수 있습니다.

### 3.4 선택 셀 편집

`SubItemHitTest()`로 클릭한 행·열 인덱스를 구하고 해당 값을 `CEdit`에 표시합니다. Edit 버튼을 누르면 편집창의 문자열을 선택한 셀에 반영하고 `Invalidate()`로 다시 그리기를 요청합니다.

```cpp
m_vecData[m_nSelRow][m_nSelCol] = strNew;
m_listCtrl.Invalidate();
```

### 3.5 CSV 저장

헤더와 각 데이터 행을 순서대로 출력합니다. 쉼표 또는 큰따옴표 등이 포함된 필드는 CSV 형식에 맞게 감싸거나 이스케이프하도록 저장 로직을 구성했습니다.

### 3.6 글꼴 변경

`CFontDialog`로 글꼴과 크기를 선택하고 `CFont::CreatePointFont()`, `SetFont()`를 이용해 리스트와 편집창에 적용합니다.



## 4. 실행 화면 (Screenshots)

> 아래 위치에 실제 프로그램 화면을 추가할 예정입니다.

### CSV 불러오기

<img width="1138" height="651" alt="image" src="https://github.com/user-attachments/assets/27c8ddf5-2080-4600-9dc6-1b09d842d229" />


### 셀 편집

<img width="1139" height="651" alt="image" src="https://github.com/user-attachments/assets/815dc460-0862-40bc-9a06-639ae8ef3067" />


### 글꼴 변경

<img width="1137" height="651" alt="image" src="https://github.com/user-attachments/assets/bf692099-75bb-460e-b3f6-8805d27ae3b3" />




## 5. 예제 데이터 (Sample Data)

테스트용 `MemberList.CSV`는 **24개 열**로 구성되어 있습니다. 카드번호, 사용자 이름, 발급 횟수, 카드 종류, 출입 스케줄 등 여러 열을 가진 예제 데이터입니다.

저장소에 예제 파일을 함께 올린다면 `samples/MemberList.CSV` 위치에 배치하여 사용할 수 있습니다.

1. 프로그램 실행 후 **Open** 클릭
2. `MemberList.CSV` 선택
3. 데이터 조회 및 셀 선택
4. **Edit**로 값 수정
5. **Save**로 CSV 파일 저장

> Excel 통합문서(`.xlsx`)를 직접 읽는 기능은 구현하지 않았으며, CSV 파일을 대상으로 합니다.



## 6. 개발 과정 및 개선 사항 (Development & Improvements)

### 6.1 데이터 구조 개선

- **기존:** `Person` 클래스에 9개 열을 고정하고 `switch` 등으로 필드별 처리
- **변경:** 헤더와 2차원 벡터로 전체 열을 관리하고 인덱스로 셀에 접근
- **결과:** 열 개수가 다른 CSV에도 같은 조회·편집 구조를 적용할 수 있도록 개선

### 6.2 CSV 파싱 오류 수정

헤더의 `"유,무상/신분증"`처럼 **큰따옴표 내부에 쉼표가 포함된 필드**가 두 개의 열로 분리되는 문제를 확인했습니다.

큰따옴표 상태를 관리하는 `bQuote` 값의 반전 처리 오류를 수정하여, 인용된 필드 내부의 쉼표를 구분자로 처리하지 않도록 했습니다.

### 6.3 파일 스트림 오류 수정

열 개수 불일치 시 오류 처리 코드에서 파일을 닫은 후 함수가 종료되지 않아 `ReadString()`이 다시 호출되는 문제가 있었습니다. 조건문과 `return`을 바로잡아 닫힌 파일을 다시 읽지 않도록 수정했습니다.

### 6.4 향후 개선 사항

- 다양한 CSV 문자 인코딩(UTF-8 등) 지원 및 검증
- 잘못된 CSV 형식에 대한 상세 오류 안내
- 데이터 검색 및 정렬 기능 추가
- 입력값 검증 및 저장 후 재불러오기 테스트 확대
- 대용량 데이터 처리 성능 검증



## 7. 빌드 및 실행 (Build & Run)

1. Windows에서 MFC 개발 구성 요소가 설치된 Microsoft Visual Studio를 실행합니다.
2. 프로젝트의 솔루션 파일(`.sln`)을 엽니다.
3. 프로젝트를 빌드하고 실행합니다.
4. **Open** 버튼으로 CSV 파일을 선택합니다.



## 8. 프로젝트 회고 (Retrospective)

이번 프로젝트를 통해 MFC의 메시지 처리 방식, `CListCtrl`의 OwnerData 동작, CSV 파일 입출력 및 데이터 편집을 학습했습니다.

특히 9개 열로 고정된 구조를 가변 열 구조로 변경하면서 **자료구조 설계가 확장성과 유지보수성에 미치는 영향**을 체감했습니다. 또한 실제 실행 중 발생한 CSV 파싱 오류와 파일 스트림 문제를 분석하고 수정하는 경험을 쌓았습니다.

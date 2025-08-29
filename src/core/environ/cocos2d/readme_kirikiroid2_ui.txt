C:\work\krkr\kirikiroid2lite-master\cocos2d-x-3.6\cocos\editor-support\cocostudio\ActionTimeline\CSLoader.cpp
if (suffix == "csb")
3 breakpoints:
Node* CSLoader::createNode(const std::string& filename)
b -> Node* CSLoader::createNode(const std::string &filename, const ccNodeLoadCallback &callback)
ActionTimeline* CSLoader::createTimeline(const std::string &filename)

C:\work\krkr\kirikiroid2lite-master\src\core\environ\ui\BaseForm.cpp
breakpoints: 
b -> Node* CSBReader::Load(const char *filename) {

b CSLoader.cpp:275

====

C:\work\krkr\kirikiroid2lite-master\src\core\environ\ui\MainFileSelectorForm.cpp
TVPMainScene::GetInstance()->pushUIForm(TVPMainFileSelectorForm::create());
inherit::initFromFile(FileName_NaviBar, FileName_Body, nullptr/*FileName_BottomBar*/, _fileList);
Node *root = reader.Load("ui/MainFileSelector.csb");
ui/MainFileSelector.csb

inherit::initFromFile two params (1/2)
C:\work\krkr\kirikiroid2lite-master\src\core\environ\ui\MainFileSelectorForm.cpp
inherit::initFromFile(FileName_NaviBar, FileName_Body, nullptr/*FileName_BottomBar*/, _fileList);
C:\work\krkr\kirikiroid2lite-master\src\core\environ\ui\BaseForm.cpp
NaviBar.Root = reader.Load(navibar);
ui/NaviBarWithMenu.csb

inherit::initFromFile two params (2/2)
C:\work\krkr\kirikiroid2lite-master\src\core\environ\ui\BaseForm.cpp
RootNode = static_cast<Widget*>(reader.Load(body));
ui/TableView.csb

C:\work\krkr\kirikiroid2lite-master\src\core\environ\ui\MainFileSelectorForm.cpp
ListHistory(); // filter history data
C:\work\krkr\kirikiroid2lite-master\src\core\environ\ui\MainFileSelectorForm.cpp
_root = reader.Load("ui/RecentListItem.csb");
ui/RecentListItem.csb
ui/RecentListItem.csb
ui/RecentListItem.csb

C:\work\krkr\kirikiroid2lite-master\src\core\environ\ui\MainFileSelectorForm.cpp
ListDir(lastpath); // getCurrentDir()
C:\work\krkr\kirikiroid2lite-master\src\core\environ\ui\FileSelectorForm.cpp
FileList->reloadData();
C:\work\krkr\kirikiroid2lite-master\src\core\environ\ui\FileSelectorForm.cpp
_root = reader.Load(filename);
ui/FileItem.csb
ui/FileItem.csb
ui/FileItem.csb
ui/FileItem.csb



void TVPMainFileSelectorForm::show() {
#if defined(_MSC_VER)
	OutputDebugString(L"===============TVPMainFileSelectorForm::show()\n");
#endif
#ifdef _DEBUG
	TVPGL_ASM_Test();
#endif
	ListHistory(); // filter history data

->

void TVPMainFileSelectorForm::ListHistory()
{
	if (!_historyList) return;
	_historyList->removeAllChildren();
#if !defined(_MSC_VER) && !defined(ANDROID) && !defined(LINUX)
	HistoryCell *nullcell = new HistoryCell();
	nullcell->init();
#else
#if 0
	__debugbreak(); throw;
#else
	HistoryCell *nullcell = HistoryCell::create("", "", "", "");

->

void TVPMainFileSelectorForm::HistoryCell::initInfo(const std::string &fullpath, const std::string &prefix, const std::string &pathname, const std::string &filename)
{
	_fullpath = fullpath;

	CSBReader reader;
	_root = reader.Load("ui/RecentListItem.csb");



===

C:\work\krkr\kirikiroid2lite-master\src\core\environ\ui\MainFileSelectorForm.cpp
_menu = reader.Load("ui/MenuList.csb");
ui/MenuList.csb

===

C:\work\krkr\kirikiroid2lite-master\src\core\environ\ui\GameMainMenu.cpp
_root = reader.Load("ui/GameMenu.csb");
ui/GameMenu.csb


==







Thread 1 "cpp-empty-test" hit Breakpoint 1, TVPListForm::create (
    cells=std::vector of length 0, capacity 0)
    at tests/cpp-empty-test/Classes/ui/FileSelectorForm.cpp:554
554		TVPListForm *ret = new TVPListForm;
(gdb) bt
#0  TVPListForm::create (cells=std::vector of length 0, capacity 0)
    at tests/cpp-empty-test/Classes/ui/FileSelectorForm.cpp:554
#1  0x0000555555633f92 in TVPBaseFileSelectorForm::onTitleClicked (
    this=0x55555689f320, owner=0x5555568f2dc0)
    at tests/cpp-empty-test/Classes/ui/FileSelectorForm.cpp:330
#2  0x00005555556415a9 in std::__invoke_impl<void, void (TVPBaseFileSelectorForm::*&)(cocos2d::Ref*), TVPBaseFileSelectorForm*&, cocos2d::Ref*> (__f=
    @0x555556805470: (void (TVPBaseFileSelectorForm::*)(TVPBaseFileSelectorForm * const, cocos2d::Ref *)) 0x555555633bfc <TVPBaseFileSelectorForm::onTitleClicked(cocos2d::Ref*)>, __t=@0x555556805480: 0x55555689f320)



_title->setTitleText

b ListDir

//FIXME:
lastpath = "/home/wmtwmt";

===




// WindowsProject1.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "WindowsProject1.h"
#include "TemplateTest.hpp"
#include <math.h>
#include "XZip.h"

#include <unordered_map>
#include <deque>
#include <string>
#include <map>
#include <functional>

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

#if _MSC_VER
#define PlaceHolder std::_Ph
#else
#define PlaceHolder std::_Placeholder
#endif

template<int N, int...I>
struct MakeSeqs : MakeSeqs<N - 1, N - 1, I...> {};

template<int...I>
struct MakeSeqs<1, I...>
{
	template<class T, class _Fx>
	static auto bind(T&& obj, _Fx&& _Func)
	{
		return std::bind(std::forward<_Fx>(_Func), std::forward<T>(obj), PlaceHolder<I>{}...);
	}
};

template <class T, class R, typename...Args>
auto Bind(T* t, R(T::*f)(Args...))
{
	return MakeSeqs<sizeof...(Args) + 1>::bind(t, f);
}

template <class T, class R, typename...Args>
auto Bind(R(T::*f)(Args...), T* t)
{
	return MakeSeqs<sizeof...(Args) + 1>::bind(t, f);
}


void Test(request& req, response& res)
{
	int i;
	i = 0;
}

class InvokeTest
{
public:
	template<typename Function>
	void invoke(int & i, Function f)
	{
		f();
	}


	template<typename Function>
	void InvokeFun(Function f)
	{
		//map_invokers_[L"1"] = std::bind(&InvokeTest::invoke,this,std::placeholders::_1, f);
	}

	void Do()
	{
		//InvokeFun(&InvokeTest::Test);
		int i = 4;
		//map_invokers_[L"1"](i);
	}

	template<typename Function, typename... AP>
	void invoke(request& req, response& res, Function f, AP... ap)
	{

	}

	template<typename Function, typename... AP>
	void register_nonmember_func(std::wstring raw_name, Function f, const AP&... ap) {
			this->map_invokers_[raw_name] = { raw_name, std::bind(&InvokeTest::invoke<Function, AP...>, this,
				std::placeholders::_1, std::placeholders::_2, std::move(f), ap...) };
	}

	template<typename Function, typename... Ap>
	void register_handler(std::wstring name, Function&& f, const Ap&... ap) {
			register_nonmember_func(name, std::forward<Function>(f), ap...);
	}

	template<typename Function, typename... AP>
	void set_http_handler(std::wstring name, Function&& f, AP&&... ap)
	{
		register_handler(name, std::forward<Function>(f), std::forward<AP>(ap)...);
	}

	//std::map<std::wstring, std::function<void(int &i)>> map_invokers_;

	typedef std::pair<std::wstring, std::function<void(request&, response&)>> invoker_function;
	std::map<std::wstring, invoker_function> map_invokers_;
};

template<typename Fn>
void Test(int &i, Fn f)
{
	f();
}

template<typename Fn, class... Args>
void Testttt(int i, Fn f, Args&&... args)
{
	f(std::forward<Args>(args)...);
}

template<typename Fn, class... Args>
bool Testttt2( Fn f, Args&&... args)
{
	f(std::forward<Args>(args)...);
	return true;
}


void Test2()
{
	int i;
	i = 0;
}

void Test3(int n)
{
	int i;
	i = 0;
}
std::map<std::wstring, std::function<void(int)>> gmap_invokers_;

template<typename Fn, class... Args>
void Add(std::wstring name, Fn fn, Args&&... args)
{
	gmap_invokers_[name] = std::bind(&Testttt<Fn, Args...>, std::placeholders::_1, fn, std::forward<Args>(args)...);
}

template <typename R, typename... Args>
class FuncContainer
{
public:


	template<typename Fn, typename... SubArgs>
	void Add(std::wstring name, Fn fn, SubArgs&&... args)
	{
		
		[name] = std::bind(&Testttt2<Fn, SubArgs...>, fn, std::forward<SubArgs>(args)...);
	}
private:
	std::map<std::wstring, std::function<R(Args...)>> map_invokers;
};

class CTest
{
public:
	void foo1(int a, std::wstring& b, float c, double d, void* p) {};
	void foo2(int a, std::string& b, float c, double d, void* p, short f) {};
};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	CTest t;
	auto f1 = Bind(&CTest::foo1, &t);
	auto f2 = Bind(&CTest::foo2, &t);

	int bbb = 13;
	FuncContainer<bool> container;
	//container.Add(L"1", Test2);
	container.Add(L"2", Test3, bbb);
	//InvokeTest t;
	//t.set_http_handler(L"a", [](request& req, response& res) {});
	//t.Do();
	
	Testttt<std::function<void(int)>, int>(bbb, Test3, 3);


	//Add(L"1", Test2);
	Add(L"2", Test3, bbb);
	//map_invokers_[L"2"] = std::bind(&Testttt<std::function<void(int)>, int>, std::placeholders::_1, Test3, 2);
	//int temp = 3;
	//map_invokers_[L"1"](temp);
	//map_invokers_[L"2"](temp);

	TCHAR zipname[] = L"E:\\Inno Setup 5";
	TCHAR zipfile[FILENAME_MAX];
	wsprintf(zipfile, L"%s.zip", zipname);

	HZIP hz = CreateZip(zipfile, 0, ZIP_FILENAME);
	std::deque<std::wstring>folderList;
	TCHAR folderpath[MAX_PATH] = L"E:\\Inno Setup 5";
	folderList.push_back(std::wstring(folderpath));
	while (folderList.size() > 0)
	{
		WIN32_FIND_DATA finddata;
		std::wstring pathfile = folderList.front();
		std::wstring subdir = pathfile;
		pathfile.append(L"\\*.*");
		HANDLE handleFind = FindFirstFile(pathfile.c_str(), &finddata); //FindFirstFile
		DWORD err = GetLastError();
		folderList.pop_front();
		if (handleFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (finddata.cFileName[0] == '.')
					continue;
				else if (finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					std::wstring subpath = subdir;
					subpath.append(L"\\");
					subpath.append(finddata.cFileName);
					folderList.push_back(subpath);
				}
				else
				{
					std::wstring filename = subdir;
					filename.append(L"\\");
					filename.append(finddata.cFileName);
					std::wstring zipdir = filename.substr(wcslen(folderpath) + 1);
					ZipAdd(hz, zipdir.c_str(), (void*)filename.c_str(), 0, ZIP_FILENAME);
				}
			} while (FindNextFile(handleFind, &finddata));
			FindClose(handleFind);
		}
	}
	CloseZip(hz);
	
	has_before_t<testhasmem> a;
	a = L"adsf";
	testhasmem t1;
	testhasmem2 t2;

	//auto aaa = is_detected< decltype(t1)>::value;
	bool has_befor_mtd = has_before<decltype(t1)>::value;
	has_befor_mtd = has_before<decltype(t2)>::value;

   	std::unordered_multimap<std::wstring, int> test;
	test.insert({ L"abc", 1 });
	test.insert({ L"bbc", 2 });
	test.insert({ L"dbc", 3 });
	test.insert({ L"abc", 4 });
	//test[L"abc"] = 1;
	//test[L"bbc"] = 2;
	//test[L"dbc"] = 3;
	//test[L"abc"] = 4;

 	auto samePair = test.equal_range(L"abc");
	for (auto i = samePair.first; i != samePair.second; i++)
	{
		i->first;
	}

	Invoke<LoggingAspect, TimeElapsedAspect>(&foo, 1); //织入方法

	Invoke<TimeElapsedAspect, LoggingAspect>(&foo, 1);

	//HandEvent(Do, &person::foo1, log_t{});
	/*auto tp = filter(&person::foo1, log_t{});
	auto lm = [](auto... ap) {
		return;
	};
	std::apply(lm, std::move(tp));*/

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

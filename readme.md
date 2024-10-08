﻿پروژه درس سیستم‌عامل 

زینب آرین‌منش
توضیح بخش‌های مختلف پروژه و مراحل پیاده‌سازی:

تعریف مقادیر ثابت و متغیرها

#DEFINE MAX\_PATH 1024

این خط یک مقدار ثابت به نام MAX\_PATH با مقدار 1024 تعریف می‌کند. این مقدار حداکثر طول مسیری است که می‌توان در این برنامه استفاده کرد. این برای ذخیره‌سازی مسیرهای فایل‌ها و دایرکتوری‌ها استفاده می‌شود.

#DEFINE MAX\_THREADS 100

این خط یک مقدار ثابت به نام MAX\_THREADS با مقدار 100 تعریف می‌کند. این مقدار نشان‌دهنده حداکثر تعداد نخ‌هایی است که می‌توانند همزمان در هر دایرکتوری برای پردازش فایل‌ها ایجاد شوند.

تعریف متغیرهای سراسری

PTHREAD\_MUTEX\_T MUTEX = PTHREAD\_MUTEX\_INITIALIZER;

این خط یک متغیر از نوع PTHREAD\_MUTEX\_T به نام MUTEX تعریف و مقداردهی اولیه می‌کند. این متغیر برای همگام‌سازی دسترسی به منابع مشترک بین نخ‌ها استفاده می‌شود. PTHREAD\_MUTEX\_INITIALIZER یک مقداردهی اولیه استاندارد برای MUTEX است.

SEM\_T SEM;

این خط یک متغیر از نوع SEM\_T به نام SEM تعریف می‌کند. این متغیر برای مدیریت همگام‌سازی و کنترل همزمانی بین فرآیندهای والد و فرزند استفاده می‌شود.

FILEINFO FILES[MAX\_THREADS];

یک آرایه از ساختار FILEINFO به نام FILES با اندازه MAX\_THREADS تعریف می‌کند. این آرایه برای ذخیره‌سازی اطلاعات فایل‌هایی که در هر دایرکتوری پردازش می‌شوند، استفاده می‌شود.



INT FILE\_COUNT = 0;

یک متغیر از نوع INT به نام FILE\_COUNT با مقدار اولیه 0 تعریف می‌کند. این متغیر برای شمارش تعداد فایل‌هایی که در هر دایرکتوری پردازش می‌شوند، استفاده می‌شود.

INT TOTAL\_FILES = 0;

یک متغیر از نوع INT به نام TOTAL\_FILES با مقدار اولیه 0 تعریف می‌کند. این متغیر برای شمارش تعداد کل فایل‌هایی که در طول اجرای برنامه پردازش می‌شوند، استفاده می‌شود.

INT DELETED\_FILES\_COUNT = 0;

یک متغیر از نوع INT به نام DELETED\_FILES\_COUNT با مقدار اولیه 0 تعریف می‌کند. این متغیر برای شمارش تعداد فایل‌هایی که به عنوان فایل تکراری شناسایی و حذف می‌شوند، استفاده می‌شود.

OFF\_T INITIAL\_SIZE = 0;

یک متغیر از نوع OFF\_T به نام INITIAL\_SIZE با مقدار اولیه 0 تعریف می‌کند. این متغیر برای ذخیره‌سازی حجم اولیه کل فایل‌ها قبل از حذف فایل‌های تکراری استفاده می‌شود.

OFF\_T FINAL\_SIZE = 0;

یک متغیر از نوع OFF\_T به نام FINAL\_SIZE با مقدار اولیه 0 تعریف می‌کند. این متغیر برای ذخیره‌سازی حجم کل فایل‌ها بعد از حذف فایل‌های تکراری استفاده می‌شود.

توابع

VOID ADD\_FILE\_SIZE(CONST CHAR\* PATH, OFF\_T\* SIZE)

این تابع برای افزودن حجم یک فایل به یک متغیر استفاده می‌شود. با استفاده از STAT، اطلاعات فایل خوانده شده و حجم آن به متغیر SIZE اضافه می‌شود.

VOID\* PROCESS\_FILE(VOID\* ARG)

این تابع برای پردازش یک فایل استفاده می‌شود. اگر فایل تکراری باشد، یکی از فایل‌های تکراری حذف می‌شود.



VOID PROCESS\_DIRECTORY(CONST CHAR\* DIR\_PATH)

این تابع برای پردازش یک دایرکتوری استفاده می‌شود. فایل‌های موجود در دایرکتوری را شناسایی و اطلاعات آن‌ها را ذخیره می‌کند.

VOID\* PROCESS\_SUBDIRECTORY(VOID\* ARG)

این تابع برای پردازش یک زیردایرکتوری استفاده می‌شود. فایل‌های موجود در زیردایرکتوری را پردازش می‌کند و اطلاعات مربوط به آن‌ها را در فایل لاگ ذخیره می‌کند.

VOID TRAVERSE\_DIRECTORIES(CONST CHAR\* BASE\_PATH)

این تابع برای پیمایش دایرکتوری‌ها و فراخوانی تابع پردازش زیردایرکتوری‌ها استفاده می‌شود. برای هر زیردایرکتوری، یک فرآیند فرزند ایجاد می‌شود.

INT MAIN()

این تابع نقطه شروع برنامه است. مسیر دایرکتوری را از کاربر می‌گیرد، متغیرهای همگام‌سازی را مقداردهی اولیه کرده و تابع پیمایش دایرکتوری‌ها را فراخوانی می‌کند. در نهایت، خروجی مورد نظر را نمایش می‌دهد.

خلاصه عملکرد کلی برنامه

ورودی: مسیر دایرکتوری را از کاربر می‌گیرد.

پردازش: دایرکتوری‌ها را پیمایش می‌کند و برای هر زیردایرکتوری یک فرآیند فرزند ایجاد می‌کند. سپس در هر فرآیند فرزند، فایل‌های موجود در زیردایرکتوری را پردازش می‌کند.

تشخیص تکراری‌ها: فایل‌های تکراری را شناسایی و یکی از آن‌ها را حذف می‌کند.

خروجی: اطلاعات مربوط به فایل‌های پردازش شده را در فایل لاگ ذخیره کرده و اطلاعات کلی را در خروجی نمایش می‌دهد.

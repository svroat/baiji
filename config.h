/*  Ahmia */
#define AHM_ENG "https://ahmia.fi"
#define AHM_OPT "/search?q=%s"
#define AHM_TIT "//li[@class='result']/h4/a"
#define AHM_URL "//li[@class='result']/h4/a/@href"
#define AHM_PAR "//li[@class='result']/p"

/*  Amazon */
#define AMZ_ENG "https://www.amazon.com"
#define AMZ_OPT "/s/keywords=%s&page=%d"
#define AMZ_TIT "//li[@class='s-result-item celwidget ']/div/div/div/div[2]/div[@class='a-row a-spacing-small']"
#define AMZ_URL "//li[@class='s-result-item celwidget ']/div/div/div/div[2]/div[@class='a-row a-spacing-small']/div[1]/a/@href"
#define AMZ_PAR "//li[@class='s-result-item celwidget ']/div/div/div/div[2]/div[@class='a-row']/div[2]"

/*  Archive */
#define ARI_ENG "https://archive.is"
#define ARI_OPT "/%s"
#define ARI_TIT "//div[@class='TEXT-BLOCK']/a[1]"
#define ARI_URL "//div[@class='TEXT-BLOCK']/a[1]/@href"
#define ARI_PAR "//div[@class='TEXT-BLOCK']/*[2]"

/*  Bing */
#define BNG_ENG "https://www.bing.com"
#define BNG_OPT "/search?q=%s&first=%d"
#define BNG_TIT "//li[@class='b_algo']/.//h2/a"
#define BNG_URL "//li[@class='b_algo']/.//h2/a/@href"
#define BNG_PAR "//li[@class='b_algo']/.//p"

/*  Dailymotion  */
#define DLM_ENG "https://dailymotion.com"
#define DLM_OPT "/relevance/search/%s/%d"
#define DLM_TIT "//div[@class='media-block']/h3/a"
#define DLM_URL "//div[@class='media-block']/h3/a/@href"
#define DLM_PAR "//div[@class='media-block']/div[@class='owner']"

/*  Digbt  */
#define DIG_ENG "https://www.digbt.org"
#define DIG_OPT "/search/%s-time-%d"
#define DIG_TIT "//td[@class='x-item']/div[1]/a"
#define DIG_URL "//td[@class='x-item']/div[1]/a/@href"
#define DIG_PAR "//td[@class='x-item']/div[@class='tail']/a/@href"

/*  Libgen  */
#define LBG_ENG "http://libgen.io/"
#define LBG_OPT "search.php?&req=%s&page=%d"
#define LBG_TIT "/html/body/table[@class='c']/.//td/a[@title='']"
#define LBG_URL "/html/body/table[@class='c']/.//td/a[@title='']/@href"
#define LBG_PAR "/html/body/table[@class='c']/.//td/a[@title='Libgen.io']/@href"

/*  Piratebay  */
#define PTB_ENG "https://thepiratebay.org"
#define PTB_OPT "/s/?q=%s&category=0&page=%d&orderby=99"
#define PTB_TIT "//tr/td[2]/div[@class='detName']/a/@href"
#define PTB_URL "//tr/td[2]/a[1]/@href"
#define PTB_PAR "//tr/td[2]/font[@class='detDesc']"

/*  Searchcode */
#define SCD_ENG "https://searchcode.com"
#define SCD_OPT "/?q=%s&p=%d"
#define SCD_TIT "//div[@class='code-result']/div/h5/a"
#define SCD_URL "//div[@class='code-result']/div/h5/a/@href"
#define SCD_PAR "//div[@class='code-result']/div/h5/*[2]"

/*  Searx */      
#define SRX_ENG "https://searx.laquadrature.net"
#define SRX_OPT "/?q=%s&categories=general&pageno=%d"
#define SRX_TIT "//div[@class='result result-default']/h4/a"
#define SRX_URL "//div[@class='result result-default']/h4/a/@href"
#define SRX_PAR "//div[@class='result result-default']/*[2]"

/* Wikipedia */
#define WPD_ENG "https://en.wikipedia.org"
#define WPD_OPT "/w/index.php?search=%s&fulltext=1&limit=20&offset=%d"
#define WPD_TIT "//ul[@class='mw-search-results']/li/div[1]/a"
#define WPD_URL "//ul[@class='mw-search-results']/li/div[1]/a/@href"
#define WPD_PAR "//ul[@class='mw-search-results']/li/div[2]"

/* Yahoo */
#define YAH_ENG "https://search.yahoo.com"
#define YAH_OPT "/search?p=%s&b=%d&pz=10"
#define YAH_TIT "//div[contains(concat(' ', normalize-space(@class), ' '), ' Sr ')]/div[1]/h3/a"
#define YAH_URL "//div[contains(concat(' ', normalize-space(@class), ' '), ' Sr ')]/div[1]/h3/a/@href"
#define YAH_PAR "//div[contains(concat(' ', normalize-space(@class), ' '), ' Sr ')]/div[2]"

/* Youtube */
#define YTB_ENG "https://www.youtube.com"
#define YTB_OPT "/results?q=%s&p=%d"
#define YTB_TIT "//div[@class='yt-lockup-content']/h3/a"
#define YTB_URL "//div[@class='yt-lockup-content']/h3/a/@href"
#define YTB_PAR "//div[@class='yt-lockup-content']/*[3]"

static const Engine_comp enginesdef[LastEngine] = {
    /* Eng           Eng. full  Eng. opt.  XPaths */
    [Ahmia]        = { AHM_ENG, AHM_OPT, { AHM_TIT, AHM_URL, AHM_PAR } },
    [Amazon]       = { AMZ_ENG, AMZ_OPT, { AMZ_TIT, AMZ_URL, AMZ_PAR } },
    [ArchiveIs]    = { ARI_ENG, ARI_OPT, { ARI_TIT, ARI_URL, ARI_PAR } },
    [Bing]         = { BNG_ENG, BNG_OPT, { BNG_TIT, BNG_URL, BNG_PAR } },
    [Dailymotion]  = { DLM_ENG, DLM_OPT, { DLM_TIT, DLM_URL, DLM_PAR } },
    [Digbt]        = { DIG_ENG, DIG_OPT, { DIG_TIT, DIG_URL, DIG_PAR } },
    [Libgen]       = { LBG_ENG, LBG_OPT, { LBG_TIT, LBG_URL, LBG_PAR } },
    [Piratebay]    = { PTB_ENG, PTB_OPT, { PTB_TIT, PTB_URL, PTB_PAR } },
    [Searchcode]   = { SCD_ENG, SCD_OPT, { SCD_TIT, SCD_URL, SCD_PAR } },
    [Searx]        = { SRX_ENG, SRX_OPT, { SRX_TIT, SRX_URL, SRX_PAR } },
    [Wikipedia]    = { WPD_ENG, WPD_OPT, { WPD_TIT, WPD_URL, WPD_PAR } },
    [Yahoo]        = { YAH_ENG, YAH_OPT, { YAH_TIT, YAH_URL, YAH_PAR } },
    [Youtube]      = { YTB_ENG, YTB_OPT, { YTB_TIT, YTB_URL, YTB_PAR } }, 
    [ErrorEngine]  = { NULL,    NULL,    { NULL,    NULL,    NULL } } 
};

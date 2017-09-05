/* See LICENSE file for copyright and license details.
 *
 * Baiji is just a simple C scrapper.
 * The XPaths are in the config.h file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdarg.h>
#include <string.h>

#include <curl/curl.h>
#include <libxml2/libxml/tree.h>
#include <libxml2/libxml/xpath.h>
#include <libxml2/libxml/HTMLparser.h>

#include "arg.h"

#define BUFFER_DEF   (1024 * 1024)
#define SEARCH_DEF   "Rick Roll'd"
#define NUMPAGES_DEF "1"

typedef enum {
    Ahmia,
    Amazon,
    ArchiveIs,
    Bing,
    Dailymotion,
    Digbt,
    Libgen,
    Piratebay,
    Searchcode,
    Searx,
    Wikipedia,
    Yahoo,
    Youtube,
    ErrorEngine,
    LastEngine
} EngineName;

typedef struct {
    char *Title;
    char *Url;
    char *Parag;
} XPaths_comp;

typedef struct {
    char *Full;
    char *Opt;
    XPaths_comp XPath;
} Engine_comp;

typedef struct {
    char *search;
    long numpages;
    Engine_comp engine;
} Url_comp;

typedef struct {
    char *data;
    int pos;
} Write_result;

typedef struct {
    FILE *fp;
    char name[26];
} File_tmp;

static Url_comp setDefault(void);
static void usage(void);
static Engine_comp getEngine(const char *engine);
static void printEngines(void);
static int getNpages(const char *npages);
static char *getSearch(const char *srx);
static void setup(const Url_comp url_user);
static int makeJobBj(const Url_comp url_user, const char *search_enc);
static int limitPage(const int npages, const char *engine);
static void printHeader(const char *search, const int lpage,
                        const char *engineF, const char *search_enc);
static int getNumParam(const int currentN, const char *engine);
static char *callCurl(const char *url);
static size_t wrtCurl(void *ptr, size_t size, size_t nmemb, void *stream);
static int makeScrap(const char *buffer, const Engine_comp eng);
static htmlDocPtr parseHtmlDoc(const char *filename);
static void printToTerm(xmlXPathObjectPtr titles, xmlXPathObjectPtr urls,
                          xmlXPathObjectPtr parags, const char *url);

#include "config.h"

char *argv0;

Url_comp 
setDefault(void)
{
    Url_comp urlTmp = {
        .numpages = getNpages("1"),
        .engine   = getEngine("srx"),
    };

    return urlTmp;
}

Engine_comp
getEngine(const char *engine)
{
    if (strcmp(engine, "ahm") == 0) {
        return enginesdef[Ahmia];
    } else if (strcmp(engine, "amz") == 0) {
        return enginesdef[Amazon];
    } else if (strcmp(engine, "ari") == 0) {
        return enginesdef[ArchiveIs];
    } else if (strcmp(engine, "bng") == 0) {
        return enginesdef[Bing];
    } else if (strcmp(engine, "dlm") == 0) {
        return enginesdef[Dailymotion];
    } else if (strcmp(engine, "dig") == 0) {
        return enginesdef[Digbt];
    } else if (strcmp(engine, "lbg") == 0) {
        return enginesdef[Libgen];
    } else if (strcmp(engine, "ptb") == 0) {
        return enginesdef[Piratebay];
    } else if (strcmp(engine, "scd") == 0) {
        return enginesdef[Searchcode];
    } else if (strcmp(engine, "srx") == 0) {
        return enginesdef[Searx];
    } else if (strcmp(engine, "wpd") == 0) {
        return enginesdef[Wikipedia];
    } else if (strcmp(engine, "yah") == 0) {
        return enginesdef[Yahoo]; 
    } else if (strcmp(engine, "ytb") == 0) {
        return enginesdef[Youtube]; 
    } else {
	fprintf(stderr, "%s%s%s\n",
                       "[Error] Not known engine shortcut "
                       "(", engine, "). Use the opt -l for see the "
                       "shortcuts of engines suppported.");
    }

    return enginesdef[ErrorEngine];
}

int
getNpages(const char *npages)
{
    Url_comp tmp_url;
    char *ep_strtol;

    if (npages) {
        tmp_url.numpages = strtol(npages, &ep_strtol, 10);
        if (tmp_url.numpages < 1 || tmp_url.numpages > 500) {
            fprintf(stderr, "%s\n", "[Error] get_npages(). "
                                    "Number out of range (1-500).");
            return 0;
        }
    }

    return tmp_url.numpages;
}

char *
getSearch(const char *srx)
{
    Url_comp tmp_url;

    if (srx) {
        if (asprintf(&tmp_url.search, "%s", srx) == -1) {
            fprintf(stderr, "%s\n", "[Error] get_search().");
            return NULL;
        }
    }

    return tmp_url.search;
}

void
usage(void)
{
    fprintf(stderr, "%s\n", "usage: buscador [-l] [-e engine] " 
                            "[-n number of pages] [search].");
    exit(1);
}

void
printEngines(void)
{
    fprintf(stderr,"Shortcuts of engines:\n\n"
                   "\tahm: ahmia.fi\n"
                   "\tamz: amazon.com\n"
                   "\tari: archive.is\n"
                   "\tbng: bing.com\n"
                   "\tdlm: dailymotion.com\n"
                   "\tdig: digbt.org\n"
                   "\tlbg: libgen.io\n"
                   "\tptb: thepiratebay.org\n"
                   "\tscd: searchcode.com\n"
                   "\tsrx: searx.laquadrature.net\n"
                   "\twpd: en.wikipedia.org\n"
                   "\tyah: search.yahoo.com\n"
                   "\tytb: youtube.com\n");
    exit(1);
}

void
setup(const Url_comp url_user)
{
    CURL *curl = curl_easy_init();
    char *search_enc = NULL;
    int  errorBaiji  = 0;

    if (!curl) {
	fprintf(stderr, "%s\n", "[Error] setup, curl_easy_init().");
	goto error_setup;
    }

    if (!url_user.search || !url_user.numpages ||
        !url_user.engine.Full)
        goto error_setup;

    if (!(search_enc = curl_easy_escape(curl, url_user.search, 0))) {
	fprintf(stderr, "%s\n", "[Error] setup, curl_easy_escape().");
	goto error_setup;
    }

    if ((errorBaiji = makeJobBj(url_user, search_enc)))
        goto error_setup;

    curl_free(search_enc);
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return;

error_setup:
    if (search_enc) curl_free(search_enc);
    if (curl)       curl_easy_cleanup(curl);
    curl_global_cleanup();
    return;
}

int 
makeJobBj(const Url_comp url_user, const char *search_enc)
{
    int i = 0;
    int lpage = 0;
    int numParam = i;
    char *urlTmp = NULL;
    char *urlForDown = NULL;
    char *pageDown = NULL;
    int errorScrap = 0;

    if ((asprintf(&urlTmp, "%s%s",
                  url_user.engine.Full, url_user.engine.Opt) == -1))
        goto error_mkjob;

    lpage = limitPage(url_user.numpages, url_user.engine.Full);
    printHeader(url_user.search, lpage, url_user.engine.Full, search_enc);
    
    for (i = 1; i <= lpage; i++) {
        numParam = getNumParam(i, url_user.engine.Full);
       
        if ((asprintf(&urlForDown, urlTmp, search_enc, numParam) == -1))
            goto error_mkjob;

        fprintf(stdout, "\n%s%d\n[%s]\n", "Page #", i, urlForDown);

        if (!(pageDown = callCurl(urlForDown)))
            goto error_mkjob;

        if ((errorScrap = makeScrap(pageDown, url_user.engine)))
            goto error_mkjob;
    }
    
    free(pageDown);
    free(urlForDown);
    free(urlTmp);
    return 0;

error_mkjob:
    if (pageDown)   free(pageDown);
    if (urlForDown) free(urlForDown);
    if (urlTmp)     free(urlTmp);
    return 1;
}

int 
limitPage(const int npages, const char *engine)
{
    int limit = 1;

    if (strcmp(engine, AHM_ENG) == 0) 
        limit = 1;
    else if (strcmp(engine, AMZ_ENG) == 0) 
        limit = 20;
    else if (strcmp(engine, ARI_ENG) == 0) 
        limit = 1;
    else if (strcmp(engine, BNG_ENG) == 0) 
        limit = 100;
    else if (strcmp(engine, DLM_ENG) == 0) 
        limit = 100;
    else if (strcmp(engine, DIG_ENG) == 0) 
        limit = 100;
    else if (strcmp(engine, LBG_ENG) == 0) 
        limit = 40;
    else if (strcmp(engine, PTB_ENG) == 0) 
        limit = 35;
    else if (strcmp(engine, SCD_ENG) == 0) 
        limit = 50;
    else if (strcmp(engine, SRX_ENG) == 0) 
        limit = 50;
    else if (strcmp(engine, WPD_ENG) == 0) 
        limit = 500;
    else if (strcmp(engine, YAH_ENG) == 0) 
        limit = 80;
    else if (strcmp(engine, YTB_ENG) == 0) 
        limit = 80;

    if (limit == 1) {
        fprintf(stderr, 
                "%s%s%s\n", 
                "[Warning] Baiji doen't support pagging of (", engine,
                "), using default.");

        return 1;
    }

    if (npages > limit) {
        fprintf(stderr, 
                "%s%s%s%d.\n", 
                "[Warning] Off the limit of the number pages for (", engine,
                "), using ", limit);
        return limit;
    } else {
        return npages;
    }
}
void 
printHeader(const char *search, const int lpage,
            const char *engineF, const char *search_enc)
{
    fprintf(stdout, "%s", "####################################################\n");
    fprintf(stdout, "\t[ BAIJI ]\n");
    fprintf(stdout, "Search        : %s\n" , search);
    fprintf(stdout, "Num. of pages : %d\n" , lpage);
    fprintf(stdout, "Engine        : %s\n" , engineF);
    fprintf(stdout, "Search enc.   : %s\n" , search_enc);
    fprintf(stdout, "%s", "####################################################\n");
}

int 
getNumParam(const int currentN, const char *engine)
{
    int nparam = 0;

    if (strcmp(engine, AHM_ENG) == 0) 
        nparam = currentN;
    else if (strcmp(engine, AMZ_ENG) == 0) 
        nparam = currentN;
    else if (strcmp(engine, ARI_ENG) == 0)
        nparam = currentN;
    else if (strcmp(engine, BNG_ENG) == 0) 
        nparam = ((currentN - 1) * 10) + 1;
    else if (strcmp(engine, DLM_ENG) == 0) 
        nparam = currentN;
    else if (strcmp(engine, DIG_ENG) == 0) 
        nparam = currentN;
    else if (strcmp(engine, LBG_ENG) == 0) 
        nparam = currentN;
    else if (strcmp(engine, PTB_ENG) == 0) 
        nparam = currentN - 1; 
    else if (strcmp(engine, SCD_ENG) == 0) 
        nparam = currentN - 1;
    else if (strcmp(engine, SRX_ENG) == 0) 
        nparam = currentN;
    else if (strcmp(engine, WPD_ENG) == 0) 
        nparam = currentN * 20;
    else if (strcmp(engine, YAH_ENG) == 0) 
        nparam = ((currentN - 1) * 10) + 1;
    else if (strcmp(engine, YTB_ENG) == 0) 
        nparam = currentN;
    
    return nparam;
}

char *
callCurl(const char *url)
{
    CURL *curl = curl_easy_init();
    char *buffer = NULL;
    long serverResponse;
    double totalTime;
    CURLcode status;

    if (!curl) {
        fprintf(stderr, "%s\n", "[Error] callCurl, curl_easy_init().");
        goto error_callcurl;
    }

    if (!(buffer = malloc(BUFFER_DEF))) {
        fprintf(stderr, "%s\n", "[Error] callCurl, malloc().");
        goto error_callcurl;
    }

    Write_result res_a = { .data = buffer, .pos = 0 };
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Baiji/1.0");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, wrtCurl);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &res_a);

    if ((status = curl_easy_perform(curl)) != 0) {
        fprintf(stderr, "%s\n", "[Error] callCurl, curl_easy_perform().");
        goto error_callcurl;
    }

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &serverResponse);

    if (serverResponse != 200) {
        fprintf(stderr, "%s%li\n", "[Error] callCurl, response: ", serverResponse);
        goto error_callcurl;
    }

    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &totalTime);
    fprintf(stderr, "%s%f%s\n", "[ ", totalTime, " s. ]");

    curl_easy_cleanup(curl);
    buffer[res_a.pos] = '\0';
    return buffer;

error_callcurl:
    if (buffer) free(buffer);
    if (curl)   curl_easy_cleanup(curl);
    return NULL;
}

size_t
wrtCurl(void *ptr, size_t size, size_t nmemb, void *stream)
{
    Write_result *result = (Write_result *)stream;

    if (result->pos + size * nmemb >= BUFFER_DEF - 1) {
	fprintf(stderr, "%s\n", "Error: writeCurl(), buffer limitado.");
	return 0;
    }

    memcpy(result->data + result->pos, ptr, size *nmemb);
    result->pos += size * nmemb;

    return size * nmemb;
}

int 
makeScrap(const char *buffer, const Engine_comp eng)
{
    LIBXML_TEST_VERSION

    htmlDocPtr doc;
    xmlXPathContextPtr ctxt;
    const xmlChar *titlepath = (xmlChar *) eng.XPath.Title;
    const xmlChar *urlpath   = (xmlChar *) eng.XPath.Url;
    const xmlChar *paragpath = (xmlChar *) eng.XPath.Parag;
    xmlXPathObjectPtr titles;
    xmlXPathObjectPtr urls;
    xmlXPathObjectPtr parags;

    if (!(doc = parseHtmlDoc(buffer)))  goto error_scrap;

    if (!(ctxt = xmlXPathNewContext(doc))) {
        fprintf(stderr, "%s\n", "[Error] makeScrap, xmlXPathNewContext().");
        goto error_scrap;
    }

    if (!(titles = xmlXPathEvalExpression(titlepath, ctxt))) {
        fprintf(stderr, "%s\n", "[Error] makeScrap, "
                                "xmlXPathEvalExpression() with titles.");
        goto error_scrap;
    }

    if (!(urls = xmlXPathEvalExpression(urlpath, ctxt))) {
        fprintf(stderr, "%s\n", "[Error] makeScrap, "
                                "xmlXPathEvalExpression() with urls.");
        goto error_scrap;
    }

    if (!(parags = xmlXPathEvalExpression(paragpath, ctxt))) {
        fprintf(stderr, "%s\n", "[Error] makeScrap, "
                                "xmlXPathEvalExpression() with parags.");
        goto error_scrap;
    }

    if (xmlXPathNodeSetIsEmpty(titles->nodesetval)) {
        fprintf(stderr, "%s\n", "+ No results (-_-;).");
        goto error_scrap;
    }

    printToTerm(titles, urls, parags, eng.Full);

    xmlXPathFreeContext(ctxt);
    xmlXPathFreeObject(titles);
    xmlXPathFreeObject(urls);
    xmlXPathFreeObject(parags);
    xmlFreeDoc(doc);
    xmlCleanupParser();
    return 0;

error_scrap:
    if (ctxt)   xmlXPathFreeContext(ctxt);
    if (titles) xmlXPathFreeObject(titles);
    if (urls)   xmlXPathFreeObject(urls);
    if (parags) xmlXPathFreeObject(parags);
    if (doc)    xmlFreeDoc(doc);
    xmlCleanupParser();
    return 1;
}

htmlDocPtr 
parseHtmlDoc(const char *buffer)
{
    htmlParserCtxtPtr ctxt;
    htmlDocPtr doc;

    if (!(ctxt = htmlNewParserCtxt())) {
        fprintf(stderr, "%s\n", "[Error] parseHtmlDoc, htmlNewParserCtxt().");
        return NULL;
    }

    doc = htmlCtxtReadMemory(ctxt,
                             buffer,
                             BUFFER_DEF,
                             "",
                             NULL,
                             HTML_PARSE_NOWARNING |
                             HTML_PARSE_NOERROR   |
                             HTML_PARSE_RECOVER);
 
    if (doc == NULL) {
        fprintf(stderr, "%s\n", "[Error] parseHtmlDoc, htmlCtxtReadMemory().");
        htmlFreeParserCtxt(ctxt);
        return NULL;
    }

    htmlFreeParserCtxt(ctxt);
    return doc;
}

void
printToTerm(xmlXPathObjectPtr titles, xmlXPathObjectPtr urls,
            xmlXPathObjectPtr parags, const char *url)
{
    int i;
    static int j = 1;
    int print_parags = 0;
    char *addToTit = NULL; 
    char *addToUrl = NULL;

    if (strcmp(url, DLM_ENG) == 0) {
        if ((asprintf(&addToTit, "%s", "")  == -1) ||
            (asprintf(&addToUrl, "%s", url) == -1))
            goto error_print_t;
    } else if (strcmp(url, DIG_ENG) == 0) {
        if ((asprintf(&addToTit, "%s", "")  == -1) ||
            (asprintf(&addToUrl, "%s", url) == -1))
            goto error_print_t;
    } else if (strcmp(url, LBG_ENG) == 0) {
        if ((asprintf(&addToTit, "%s", "")  == -1) ||
            (asprintf(&addToUrl, "%s", url) == -1))
            goto error_print_t;
    } else if (strcmp(url, PTB_ENG) == 0) {
        if ((asprintf(&addToTit, "%s", url) == -1) ||
            (asprintf(&addToUrl, "%s", "")  == -1))
            goto error_print_t;
    } else if (strcmp(url, SCD_ENG) == 0) {
        if ((asprintf(&addToTit, "%s", "")  == -1) ||
            (asprintf(&addToUrl, "%s", url) == -1))
            goto error_print_t;
    } else if (strcmp(url, WPD_ENG) == 0) {
        if ((asprintf(&addToTit, "%s", "")  == -1) ||
            (asprintf(&addToUrl, "%s", url) == -1))
            goto error_print_t;
    } else if (strcmp(url, YTB_ENG) == 0) {
        if ((asprintf(&addToTit, "%s", "")  == -1) ||
            (asprintf(&addToUrl, "%s", url) == -1))
            goto error_print_t;
    } else {
        if ((asprintf(&addToTit, "%s", "") == -1) ||
            (asprintf(&addToUrl, "%s", "") == -1))
            goto error_print_t;
    }

    if (titles->nodesetval->nodeNr == parags->nodesetval->nodeNr)
        print_parags = 1;

    for (i = 0; i < titles->nodesetval->nodeNr; i++, j++) {
        fprintf(stdout, "%s\n", 
                        "+--------------------------------------------------+");
        fprintf(stdout, "[%d] ", j);
        fprintf(stdout, "%s%s\n",
                        addToTit,
                        xmlNodeGetContent(titles->nodesetval->nodeTab[i]));

        fprintf(stdout, "%s%s\n", 
                        addToUrl,
                        xmlNodeGetContent(urls->nodesetval->nodeTab[i]));

        if (print_parags) {
            fprintf(stdout, "%s\n", 
                            xmlNodeGetContent(parags->nodesetval->nodeTab[i]));
        }

        puts("");
    }

    free(addToTit);
    free(addToUrl);
    return;

error_print_t:
    if (addToTit) free(addToTit);
    if (addToUrl) free(addToUrl);
    fprintf(stderr, "%s\n", "asprintf().");
    return;
}

int
main(int argc, char *argv[])
{
    Url_comp main_url = setDefault();

    ARGEBIN {
    case 'e':
        main_url.engine = getEngine(EARGF(usage()));
	break;
    case 'l':
	printEngines();
	break;
    case 'n':
	main_url.numpages = getNpages(EARGF(usage()));
	break;
    default:
	usage();
    } ARGEND;

    if (argc > 0) {
	main_url.search   = getSearch(argv[0]);
	setup(main_url);
    } else {
	usage();
    }

    if (main_url.search) free(main_url.search);

    return 0;
}

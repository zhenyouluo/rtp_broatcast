#include "config.h"
#include "lib_utils.h"

static INT32 load_config(global_confs_t *pConf, BOOL8 blReload)
{
    FILE        *pFileCfg;        /* Transaction */
    char         chLine[256];
    char         chSeps[] = " \t\n";
    char         chRecord[10][255];
    char        *chToken;
    char         n32RecordCount, n32TokenCount;

    // init
    memset(pConf, 0, sizeof(*pConf));

    if ( ( pFileCfg = fopen("wifi_d.conf","r") ) == NULL )
    {
        printf(" load_config() error : The file 'wifi_d.conf' could not opened \n");
    }

    n32RecordCount = 0;

    do
    {
        /* read the new Line, and delete COMMEN Line */
        if ( ( fgets(chLine,sizeof(chLine),pFileCfg) ) == NULL )
        {
            if ( n32RecordCount == 0 )
            {
                printf("load_config() error : No Data in the file.\n");
                break;     /* end of the file */
            }
            else
            {
                printf("load_config() OK : No Data yet in the file.\n");
                break;     /* all data scaned. end of the file */
            }
        }
        else
        {
            if ( (chLine[0] == '/') || (chLine[0] == '\r') || (chLine[0] == '\n') )
            {
                continue;
            }
        }

        /* Deal One record from file */
        n32TokenCount = 0;  /* protect the chRecord Array */

        chToken = strtok( chLine, chSeps );
        while( chToken != NULL )
        {
            assert( strlen(chToken) < 255 );
            assert( n32TokenCount < 10 );
            /* While there are tokens in "string" */
            strcpy( &chRecord[ n32TokenCount ][0], chToken );
            /* Get next token: */
            n32TokenCount++;
            chToken = strtok( NULL, chSeps );
        }

        if ( ( n32TokenCount < 3 ) && ( n32TokenCount!=0 ) )
        {
            printf("readCallInfo() Error : Read Incomplete Schedule Record.\n");
            printf("Line NUM = %d, Token NUM = %d\n", n32RecordCount, n32TokenCount );
            return -1;
        }

        /* change the string to data */
        strToLower( &chRecord[0][0] );
        if ( strcmp(&chRecord[0][0], "servicename") == 0 ) {
            strcpy(pConf->aSrvName, &chRecord[2][0]);
        } else if ( strcmp(&chRecord[0][0], "serviceport") == 0 ) {
            pConf->u16SrvPort = (UINT16)atoi(&chRecord[2][0]);
        } else if ( strcmp(&chRecord[0][0], "meidaport") == 0 ) {
            pConf->u16MediPort = (UINT16)atoi(&chRecord[2][0]);
        } else if ( strcmp(&chRecord[0][0], "maxconn") == 0 ) {
            pConf->u16MaxConn = (UINT16)atoi(&chRecord[2][0]);
        } else if ( strcmp(&chRecord[0][0], "f-name") == 0 ) {
            strcpy( pConf->aFName, &chRecord[2][0]);
        } else if ( strcmp(&chRecord[0][0], "moh_cn") == 0 ) {
            strcpy( pConf->aMohCN, &chRecord[2][0]);
        } else if ( strcmp(&chRecord[0][0], "moh_noconn") == 0 ) {
            strcpy( pConf->aMohNoConn, &chRecord[2][0]);
        } else if ( strcmp(&chRecord[0][0], "timer_keepalive") == 0 ) {
            pConf->u32KATimer = (UINT16)atoi(&chRecord[2][0]);
        } else if ( strcmp(&chRecord[0][0], "timer_offline") == 0 ) {
            pConf->u32OfflineTimer = (UINT16)atoi(&chRecord[2][0]);
        } else {
            printf("%s has unkown data at %d, program exit!\n", __FILE__, __LINE__ );
            return n32RecordCount;
        }

        n32RecordCount++;
        assert( n32RecordCount < 6 );

    }while ( !feof(pFileCfg) );
                          
    fclose( pFileCfg );
                          
    return n32RecordCount;
}




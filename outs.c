#include "outs.h"
/*
 * C Function wrapper for TRAP #15 function to output a character
 */
VOID rtx_dbug_out_char( CHAR c )
{
	
    /* Store registers */
    asm( "move.l %d0, -(%a7)" );
    asm( "move.l %d1, -(%a7)" );

    /* Load CHAR c into d1 */
    asm( "move.l 8(%a6), %d1" );

    /* Setup trap function */
    asm( "move.l #0x13, %d0" );
    asm( "trap #15" );

    /* Restore registers  */
    asm(" move.l %d1, (%a7)+" );
    asm(" move.l %d0, (%a7)+" );

}


/*
 * Print a C-style null terminated string
 */
SINT32 rtx_dbug_outs( CHAR* s )
{
    if ( s == NULL )
    {
        return RTX_ERROR;
    }
    while ( *s != '\0' )
    {
        rtx_dbug_out_char( *s++ );
    }
    return RTX_SUCCESS;
}

SINT32 rtx_dbug_out_int16( UINT32 num )
{
    int i = 0;
	int j = 0;
	int lsd  = 0; //least signficant digit
	char buf[10] = {'\0'};
	
	if(num == 0) 
    {
		rtx_dbug_out_char('0');
	} 
    else 
    {
		while (num!=0)
		{
			lsd = num % 16;
			buf[i] = "0123456789abcdef"[lsd];
			num = num / 16; //remove current ones digit
			i++; //length of number
		}

		for( j = i-1 ; j >= 0 ; j--)
		{
			rtx_dbug_out_char(buf[j]); 
		}
	}
    return RTX_SUCCESS;	
}

SINT32 rtx_dbug_out_int10( UINT32 num )
{
    int i = 0;
	int j = 0;
	int lsd  = 0; //least signficant digit
	char buf[10] = {'\0'};
	
	if(num == 0) 
    {
		rtx_dbug_out_char('0');
	} 
    else 
    {
		while (num!=0)
		{
			lsd = num % 10;
			buf[i] = "0123456789"[lsd];
			num = num / 10; //remove current ones digit
			i++; //length of number
		}

		for( j = i-1 ; j >= 0 ; j--)
		{
			rtx_dbug_out_char(buf[j]); 
		}
	}
    return RTX_SUCCESS;	
}



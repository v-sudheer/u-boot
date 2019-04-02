/*
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <common.h>
#include <console.h>
#include <bootretry.h>
#include <cli.h>
#include <command.h>
#include <console.h>

#include <inttypes.h>
#include <mapmem.h>
#include <asm/io.h>
#include <linux/compiler.h>
#include <asm/arch/ast-bmc-scu.h>

DECLARE_GLOBAL_DATA_PTR;

static int do_aspeed_usbtest(cmd_tbl_t *cmdtp, int flag, int argc,
			char * const argv[])
{
	ulong port, testfun = 0;
	u32 test_reg_set = 0;
	int ret = 1;

	printf("**************************************************** \n");
	printf("*** ASPEED USB Test                              *** \n"); 
	printf("*** #port : 0, 1                                 *** \n");
	printf("*** #function_test : 0. TestJ, 1.Test K,         *** \n");
	printf("***                 2. Measure HS Resistance,    *** \n");
	printf("***               	3. Measure Eye               *** \n");
	printf("**************************************************** \n"); 
	printf("\n"); 

	//printf("%d , %s %s \n", argc, argv[1], argv[2]);
	if ( argc != 3 ){
		ret = 0;
		return ( ret );
	} else {
		if (strict_strtoul(argv[1], 10, &port) < 0)
			return CMD_RET_USAGE;
		if (strict_strtoul(argv[2], 10, &testfun) < 0)
			return CMD_RET_USAGE;
	}

	//{HUB00} bit[11] = 1, disable PHY reset
	//{HUB00} bit[10:8] = required setting, 1: Test J, 2: Test K, 3: Measure HS Resistance, 4: Measure Eye
	//{HUB00} bit[6] = 1, Force USB to HS state
	//{HUB00} bit[0] = 1, Enable upstream port connection
	switch(testfun) {
		case 0:
			test_reg_set = 0x941;
			break;
		case 1:
			test_reg_set = 0xA41;
			break;
		case 2:
			test_reg_set = 0xB41;
			break;
		case 3:
			test_reg_set = 0xC41;
			break;
		default:
			return CMD_RET_USAGE;
			break;
	}

	switch(port) {
		case 0:
			//aspeed_pinctrl_group_set("USB2AD");

			//{SCU90} bit[29] = 0, select USB2.0 port 1 mode as device
			//{SCU04} bit[14] = 1, enable controller reset
			//{SCU0C} bit[14] = 1, enable PHY clock
			//wait 10 ms for PLL locking
			//{SCU04} bit[14] = 0, disable controller reset
			
			ast_scu_multi_func_usb_port1_mode(0);
			ast_scu_init_usb_port1();
			writel(test_reg_set, 0x1e6a0000);
#if 0			
			For example :
			mw 1e6e2000 1688a8a8
			mw 1e6e2090 0
			mw 1e6e2004 4000
			mw 1e6e200c eff47e8b
			mw 1e6e2004 0
			mw 1e6a0000 941 / A41 / B41 / C41
#endif			
			break;
		case 1:
			//aspeed_pinctrl_group_set("USB2BD");
			ast_scu_multi_func_usb_port2_mode(3);
			ast_scu_init_usb_port2();
#if 0		
			//{SCU94} bit[14:13] = "01", select USB2.0 port 2 mode as device
			//{SCU04} bit[3] = 1, enable controller reset
			
			For example :
			mw 1e6e2000 1688a8a8
			mw 1e6e2094 2000
			mw 1e6e2004 8
			mw 1e6e200c eff43e0b
			mw 1e6e2004 0
			mw 1e6a2000 941 / A41 / B41 / C41
#endif			
			writel(test_reg_set, 0x1e6a2000);
			break;
		default:
			return CMD_RET_USAGE;
			break;
	}

	return 0;

}


U_BOOT_CMD(
	usbtest,   5, 0,  do_aspeed_usbtest,
	"ASPEED usbtest- USB eye diag",
	""
);

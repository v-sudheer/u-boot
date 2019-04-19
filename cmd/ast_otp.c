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

DECLARE_GLOBAL_DATA_PTR;

#define OTP_PASSWD	0x349fe38a
#define RETRY		3
#define OTPCFG0_OFFSEST 0x800
#define OTPCFG1_OFFSEST 0x802
#define OTPCFG2_OFFSEST 0x804
#define OTPCFG3_OFFSEST 0x806
#define OTPCFG4_OFFSEST 0x808
#define OTPCFG5_OFFSEST 0x80a
#define OTPCFG6_OFFSEST 0x80c
#define OTPCFG7_OFFSEST 0x80e

#define OTPCFG8_OFFSEST 0xa00
#define OTPCFG9_OFFSEST 0xa02
#define OTPCFG10_OFFSEST 0xa04
#define OTPCFG11_OFFSEST 0xa06
#define OTPCFG12_OFFSEST 0xa08
#define OTPCFG13_OFFSEST 0xa0a
#define OTPCFG14_OFFSEST 0xa0c
#define OTPCFG15_OFFSEST 0xa0e

#define OTPCFG16_OFFSEST 0xc00
#define OTPCFG17_OFFSEST 0xc02
#define OTPCFG18_OFFSEST 0xc04
#define OTPCFG19_OFFSEST 0xc06
#define OTPCFG20_OFFSEST 0xc08
#define OTPCFG21_OFFSEST 0xc0a
#define OTPCFG22_OFFSEST 0xc0c
#define OTPCFG23_OFFSEST 0xc0e

#define OTPCFG24_OFFSEST 0xe00
#define OTPCFG25_OFFSEST 0xe02
#define OTPCFG26_OFFSEST 0xe04
#define OTPCFG27_OFFSEST 0xe06
#define OTPCFG28_OFFSEST 0xe08
#define OTPCFG29_OFFSEST 0xe0a
#define OTPCFG30_OFFSEST 0xe0c
#define OTPCFG31_OFFSEST 0xe0e

static int otp_read_data(uint32_t offset, uint32_t *data)
{
	writel(offset, 0x1e6f2010); //Read address
	writel(0x23b1e361, 0x1e6f2004); //trigger read
	udelay(2);
	data[0] = readl(0x1e6f2020);
	data[1] = readl(0x1e6f2024);
	return 1;
}

static int otp_read_config(uint32_t offset, uint32_t *data)
{
	int config_offset;

	config_offset = 0x800;
	config_offset |= (offset / 8) * 0x200;
	config_offset |= (offset % 8) * 0x2;

	writel(config_offset, 0x1e6f2010);  //Read address
	writel(0x23b1e361, 0x1e6f2004); //trigger read
	udelay(2);
	data[0] = readl(0x1e6f2020);

	return 1;
}

static int otp_print_config(uint32_t offset, int dw_count)
{
	int i;
	uint32_t ret[1];

	if (offset + dw_count > 32)
		return -1;
	for (i = offset; i < offset + dw_count; i ++) {
		otp_read_config(i, ret);
		printf("OTPCFG%d: %08X\n", i, ret[0]);
	}
	printf("\n");
	return 1;
}

static int otp_print_data(uint32_t offset, int dw_count)
{
	int i;
	uint32_t ret[2];

	if (offset + dw_count > 2048 || offset % 4 != 0)
		return -1;
	for (i = offset; i < offset + dw_count; i += 2) {
		otp_read_data(i, ret);
		if (i % 4 == 0)
			printf("%03X: %08X %08X ", i * 4, ret[0], ret[1]);
		else
			printf("%08X %08X\n", ret[0], ret[1]);

	}
	printf("\n");
	return 1;
}

static int otp_compare(uint32_t otp_addr, uint32_t *data)
{
	uint32_t ret;

	writel(otp_addr, 0x1e6f2010); //Compare address
	writel(data[0], 0x1e6f2020); //Compare data 1
	writel(data[1], 0x1e6f2024); //Compare data 2
	writel(data[2], 0x1e6f2028); //Compare data 3
	writel(data[3], 0x1e6f202c); //Compare data 4
	writel(0x23b1e363, 0x1e6f2004); //Compare command
	udelay(10);
	ret = readl(0x1e6f2014); //Compare command
	if (ret & 0x1)
		return 0;
	else
		return -1;
}

static void otp_write(uint32_t otp_addr, uint32_t data)
{
	writel(otp_addr, 0x1e6f2010); //write address
	writel(data, 0x1e6f2020); //write data
	writel(0x23b1e362, 0x1e6f2004); //write command
	udelay(100);
}

static void otp_prog(uint32_t otp_addr, uint32_t prog_bit)
{
	writel(otp_addr, 0x1e6f2010); //write address
	writel(prog_bit, 0x1e6f2020); //write data
	writel(0x23b1e364, 0x1e6f2004); //write command
	udelay(85);
}

static int prog_verify(uint32_t otp_addr, int bit_offset, int value)
{
	int ret;

	writel(otp_addr, 0x1e6f2010); //Read address
	writel(0x23b1e361, 0x1e6f2004); //trigger read
	udelay(2);
	ret = readl(0x1e6f2020);
	// printf("prog_verify = %x\n", ret);
	if (((ret >> bit_offset) & 1) == value)
		return 0;
	else
		return -1;
}

static int otp_prog_boot(uint32_t *buf, int otp_addr, int dw_count)
{
	int i, j, k, bit_value;
	int pass, soak;
	uint32_t prog_bit, prog_address;

	otp_write(0x3000, 0x4061); // Write MRA
	otp_write(0x5000, 0x302f); // Write MRB
	otp_write(0x1000, 0x4020); // Write MR
	for (i = 0; i < dw_count; i++) {
		prog_address = 0x800;
		prog_address |= ((i + otp_addr) / 8) * 0x200;
		prog_address |= ((i + otp_addr) % 8) * 0x2;
		for (j = 0; j < 32; j++) {
			bit_value = (buf[i] >> j) & 0x1;
			if (bit_value)
				prog_bit = ~(0x1 << j);
			else
				continue;
			pass = 0;
			soak = 0;
			for (k = 0; k < RETRY; k++) {
				if (!soak) {
					writel(0x04190760, 0x1e602008); //normal program
					otp_prog(prog_address, prog_bit);
					if (prog_verify(prog_address, j, bit_value) == 0) {
						pass = 1;
						break;
					}
					soak = 1;
				}
				writel(0x041930d4, 0x1e602008); //soak program
				otp_prog(prog_address, prog_bit);
				if (prog_verify(prog_address, j, bit_value) == 0) {
					pass = 1;
					break;
				}
			}
			if (!pass)
				return -1;
		}
	}
	return 0;
}

// static int otp_prog_strap(uint32_t *buf, int otp_addr, int dw_count)
// {
// 	int i;
// 	uint32_t OTPSTRAP[16];
// 	for (i = 0; i < 16; i++)
// 		otp_read_config(i + 16, &OTPSTRAP[i]);

// 	for (i = 0; i < 32; i++) {
// 		if ((OTPSTRAP[15] & (1 << i)) && (OTPSTRAP[15] & (1 << i))) {

// 		}
// 	}

// }

static int otp_prog_data(uint32_t *buf, int otp_addr, int dw_count)
{
	int i, j, k, bit_value;
	int pass, soak;
	uint32_t prog_bit, prog_address;

	otp_write(0x3000, 0x4061); // Write MRA
	otp_write(0x5000, 0x302f); // Write MRB
	otp_write(0x1000, 0x4020); // Write MR
	for (i = 0; i < dw_count; i++) {
		prog_address = i + otp_addr;
		for (j = 0; j < 32; j++) {
			bit_value = (buf[i] >> j) & 0x1;
			if (prog_address % 2 == 0) {
				prog_address |= 1 << 15;
				if (bit_value)
					prog_bit = ~(0x1 << j);
				else
					continue;
			} else {
				prog_address |= 1 << 15;
				// printf("bit_value = %x\n", bit_value);
				if (bit_value)
					continue;
				else
					prog_bit = 0x1 << j;
			}
			pass = 0;
			soak = 0;
			for (k = 0; k < RETRY; k++) {
				if (!soak) {
					writel(0x04190760, 0x1e602008); //normal program
					// printf("prog_address = %x\n", prog_address);
					// printf("prog_bit = %x\n", prog_bit);
					otp_prog(prog_address, prog_bit);
					if (prog_verify(prog_address, j, bit_value) == 0) {
						pass = 1;
						break;
					}
					soak = 1;
				}
				writel(0x041930d4, 0x1e602008); //soak program
				otp_prog(prog_address, prog_bit);
				if (prog_verify(prog_address, j, bit_value) == 0) {
					pass = 1;
					break;
				}
			}
			if (!pass)
				return -1;
		}
	}
	return 0;
}

// static int otp_data_confirm(uint32_t *buf){

// }

static int do_otp_prog(int mode, int addr, int otp_addr, int dw_count)
{
	int ret;
	uint32_t *buf;

	buf = map_physmem(addr, dw_count * 4, MAP_WRBACK);
	if (!buf) {
		puts("Failed to map physical memory\n");
		return 1;
	}

	if (mode == 1) {
		return otp_prog_boot(buf, otp_addr, dw_count);
	} else if (mode == 2) {
		return otp_prog_data(buf, otp_addr, dw_count);
	} else if (mode == 3) {
		ret = otp_prog_data(&buf[12], 0, dw_count - 32);
		if (ret < 0)
			return ret;
		return otp_prog_boot(buf, 0, 32);
	}
	return 0;
}
static int do_ast_otp(cmd_tbl_t *cmdtp, int flag, int argc,
		      char *const argv[])
{
	char *cmd;
	int mode = 0;
	uint32_t addr, dw_count, otp_addr;



	if (argc < 2) {
usage:
		return CMD_RET_USAGE;
	}

	cmd = argv[1];
	if (!strcmp(cmd, "read")) {
		if (!strcmp(argv[2], "conf"))
			mode = 1;
		else if (!strcmp(argv[2], "data"))
			mode = 2;
		else
			goto usage;

		writel(OTP_PASSWD, 0x1e6f2000); //password
		otp_addr = simple_strtoul(argv[3], NULL, 16);
		dw_count = simple_strtoul(argv[4], NULL, 16);
		if (mode == 1)
			otp_print_config(otp_addr, dw_count);
		else
			otp_print_data(otp_addr, dw_count);
	} else if (!strcmp(cmd, "prog")) {
		if (!strcmp(argv[2], "conf"))
			mode = 1;
		else if (!strcmp(argv[2], "data"))
			mode = 2;
		else if (!strcmp(argv[2], "raw"))
			mode = 3;
		else
			goto usage;

		writel(OTP_PASSWD, 0x1e6f2000); //password
		addr = simple_strtoul(argv[3], NULL, 16);
		otp_addr = simple_strtoul(argv[4], NULL, 16);
		dw_count = simple_strtoul(argv[5], NULL, 16);
		return do_otp_prog(mode, addr, otp_addr, dw_count);
	} else if (!strcmp(cmd, "comp")) {
		writel(OTP_PASSWD, 0x1e6f2000); //password
		addr = simple_strtoul(argv[2], NULL, 16);
		otp_addr = simple_strtoul(argv[3], NULL, 16);
	} else {
		goto usage;
	}


	return 0;
}


U_BOOT_CMD(
	astotp, 6, 0,  do_ast_otp,
	"ASPEED One-Time-Programmable sub-system",
	"read conf|data|raw <otp_addr>\n"
	"astotp prog conf|data|raw <addr> <otp_addr> <dw_count>\n"
	"astotp comp <addr> <otp_addr>"
);

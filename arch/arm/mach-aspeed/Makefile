#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#


obj-y += reset.o cpuinfo.o ast-ahbc.o ast-bmc-scu.o board.o aspeed-reset.o clk_aspeed.o pinctrl_aspeed.o aspeed_scu_info.o

obj-$(CONFIG_ASPEED_NONSECUR_MODE) += gic_init.o
obj-$(CONFIG_USE_IRQ) += interrupts.o
obj-$(CONFIG_ARCH_AST1070)	+= ast1070-scu.o

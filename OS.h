//
//  OS.h
//  VirtualFileSys
//
//  Created by darknight on 2018/4/10.
//  Copyright © 2018年 darknight. All rights reserved.
//

#ifndef OS_h
#define OS_h
#include <string>
#include <iostream>
#include <ctime>
#include<map>
using namespace std;

/*
    定义每项的大小
 */
#define SB_SIZE 64 //超级块大小
#define CURRENT_SIZE 64 //磁盘当前状态大小
#define MFD_SIZE 64 //主文件目录大小
#define MAX_USER_NUM (512 / 64) //用户个数
#define DIR_ENTRY_SIZE 32 //目录项大小
#define MAX_ENTRY_BLOCK 4 //目录最多数据块数
#define MAX_DATA_BLOCK 8 //文件最大数据块数
#define INODE_SIZE 64 //索引节点大小
#define BLOCK_SIZE 512//数据块大小

#define ROOT_ENTRY_BLOCK_NUM 1 //根目录项块数
#define INODE_BLOCK_NUM 1024 //索引节点块数
#define ENTRY_BLOCK_NUM 1024 //目录项块数
#define DATA_BLOCK_NUM 4096 //数据块个数
#define ENTRY_NUM_PER_BLOCK (512 / 32) //每块中的目录项的个数
#define INODE_NUM_PER_BLOCK (512 / 64) //每块中的索引节点个数

#define DISK_SIZE (1 + 2 + 1024 + 1024 + 4096) //磁盘大小

#define DISK_START 0
#define ROOT_DIR_START 2 //根目录项表的起始块
#define INODE_START (2+ 1) //索引节点表的起始地址
#define DIR_START (1024 + 2 + 1) //目录项区的起始地址
#define DATA_BLOCK_START (1 + 2 + 1024 + 1024) // 数据起始地址

#define VOLUME_NAME	"EXT2FS"
/*
    SUPER_BLOCK 32 , CURRENT_STATE 64 |  MFD  16 | root_dir_Entry Area 1 | INODE Area 1024 | dir_entry Area 1024 | Data Block 4096
 */

// 超级块
struct super_Block {
    string volume_name; //卷名
    unsigned short disk_size; //磁盘大小
    unsigned short data_Block_Num; //块数
    unsigned short block_Size; //块大小
};
//索引节点
struct type_Inode{
    string file_name;//文件名称
    unsigned short i_block; //数据块号
    unsigned short i_mode; //访问权限
    unsigned short i_block_num; //文件数据块个数，最大目录4*32，文件8*512
    unsigned long i_size; //文件大小
    std::time_t c_time;//创建时间
    std::time_t m_time;//修改时间
    std::time_t d_time; //删除时间
};

//用户文件目录,目录项
struct dir_Entry {
    string file_name;//可以为文件或目录
    unsigned short inodeNum; //索引号
    unsigned short name_length; //文件名长度
    unsigned short type; //类型，目录或者文件（1 文件，2 目录）
};

// 主文件目录
struct type_MFD {
    string user_name;
    string pass_word;
    unsigned short root_entry_num; //根文件目录号
};

//当前状态
struct current_State {
    string current_Path;
    string current_User;
    unsigned short current_Inode;
    unsigned short free_inode_num;
    unsigned short free_entry_num;
    unsigned short free_block_num;
    unsigned short free_user_num;
};

static type_MFD *mfd = new type_MFD; //MFD缓冲区
static char Buffer[512]; //数据块缓冲区
static map<string, unsigned short> fopenFile;//文件打开表,(文件名，文件句柄)
static super_Block *superblock = new super_Block; //超级块缓冲区
static current_State *cstate = new current_State; //目前状态缓冲区
static dir_Entry *root_dir_item = new dir_Entry; //根目录项缓冲区
static dir_Entry dir_item[16];//目录项缓冲区
static type_Inode *inode = new type_Inode; //索引节点缓冲区
static char tempFileBuf[8 * 512]; //文件缓冲区
static FILE *fp; //磁盘虚拟指针
static void intialize_disk(void);
static void load_super_block(void);
static void update_super_block(void);
static void load_mfd_block(unsigned short);
static void update_mfd_block(unsigned short);
static void load_root_entry_block(unsigned short);
static void update_root_entry_block(unsigned short);
static void update_entry_block(unsigned short);
static void load_entry_block(unsigned short);
static void load_inode_block(unsigned short);
static void update_inode_block(unsigned short);
static void load_block(unsigned short);
static void update_block(unsigned short);
static unsigned short getRootEntry(void);
static unsigned short getBlock(void);
static unsigned short getNode(void);
static unsigned short getEntry(void);
static void load_cur_block(void);
static void update_cur_block(void);
static void check_disk(void);


extern void intialize_memory(void);
extern bool login(string, string);
extern void dir(void);
extern void create(string);
extern void del(string);
extern void open(string);
extern void close(string);
extern void read(string);
extern void write(string);
extern void cd(string);
extern void rd(string);
extern void md(string);
extern void logout(void);
extern void useradd(string);
extern void su(string);
extern void help(void);
extern void ckdisk(void);
extern void format(void);
#endif /* OS_h */

//
//  FuncState.h
//  VirtualFileSys
//
//  Created by darknight on 2018/4/11.
//  Copyright © 2018年 darknight. All rights reserved.
//

#ifndef FuncState_h
#define FuncState_h
#include <stdio.h>
#include "OS.h"

static void update_block(unsigned short i) {
    fp = fopen("vf.txt", "r+b");
    fseek(fp, DATA_BLOCK_START * BLOCK_SIZE + (i - 1) * BLOCK_SIZE, SEEK_CUR);
    fwrite(Buffer, BLOCK_SIZE, 1, fp);
    fflush(fp);
    fclose(fp);
}

static void load_block(unsigned short i) {
    fp = fopen("vf.txt", "r+b");
    fseek(fp, DATA_BLOCK_START * BLOCK_SIZE + (i - 1) * BLOCK_SIZE, SEEK_CUR);
    fread(Buffer, BLOCK_SIZE, 1, fp);
    fclose(fp);
}

static void update_entry_block(unsigned short i) {
    fp = fopen("vf.txt", "r+b");
    fseek(fp, DIR_START * BLOCK_SIZE + (i - 1)* BLOCK_SIZE, SEEK_SET);
    fwrite(dir_item, BLOCK_SIZE, 1, fp);
    fflush(fp);
    fclose(fp);
}

static void load_entry_block(unsigned short i) {
    fp = fopen("vf.txt", "r+b");
    fseek(fp, DIR_START * BLOCK_SIZE + (i - 1)* BLOCK_SIZE, SEEK_SET);
    fread(dir_item, BLOCK_SIZE, 1, fp);
    fclose(fp);
}

static void update_inode_block(unsigned short i) {
    fp = fopen("vf.txt", "r+b");
    fseek(fp, INODE_START * BLOCK_SIZE + (i - 1)* INODE_SIZE, SEEK_SET);
    fwrite(inode, INODE_SIZE, 1, fp);
    fflush(fp);
    fclose(fp);
}

static void load_inode_block(unsigned short i) {
    fp = fopen("vf.txt", "r+b");
    fseek(fp, INODE_START * BLOCK_SIZE + (i - 1)* INODE_SIZE, SEEK_SET);
    fread(inode, INODE_SIZE, 1, fp);
    fclose(fp);
}

static void update_root_entry_block(unsigned short i) {
    fp = fopen("vf.txt", "r+b");
    fseek(fp,  ROOT_DIR_START * BLOCK_SIZE + (i - 1)* DIR_ENTRY_SIZE, SEEK_SET);
    fwrite(root_dir_item, DIR_ENTRY_SIZE, 1, fp);
    fflush(fp);
    fclose(fp);
}

static void load_root_entry_block(unsigned short i) {
    fp = fopen("vf.txt", "r+b");
    fseek(fp, ROOT_DIR_START * BLOCK_SIZE + (i - 1)* DIR_ENTRY_SIZE, SEEK_SET);
    fread(root_dir_item, DIR_ENTRY_SIZE, 1, fp);
    fclose(fp);
}

static void update_mfd_block(unsigned short i) {
    fp = fopen("vf.txt", "r+b");
    fseek(fp, BLOCK_SIZE + (i - 1)*MFD_SIZE, SEEK_SET);
    fwrite(mfd, MFD_SIZE, 1, fp);
    fflush(fp);
    fclose(fp);
}

static void load_mfd_block(unsigned short i) {
    fp = fopen("vf.txt", "r+b");
    fseek(fp, BLOCK_SIZE + (i - 1)*MFD_SIZE, SEEK_SET);
    fread(mfd,  MFD_SIZE, 1, fp);
    fclose(fp);
}

static void update_cur_block(void) {
    fp = fopen("vf.txt", "r+b");
    fseek(fp, SB_SIZE, SEEK_SET);
    fwrite(cstate, CURRENT_SIZE, 1, fp);
    fflush(fp);
    fclose(fp);
}

static void load_cur_block(void) {
    fp = fopen("vf.txt", "r+b");
    fseek(fp, SB_SIZE, SEEK_SET);
    fread(cstate, CURRENT_SIZE, 1, fp);
    fclose(fp);
}

static void update_super_block(void) {
    fp = fopen("vf.txt", "r+b");
    fseek(fp, DISK_START, SEEK_SET);
    fwrite(superblock, SB_SIZE, 1, fp);
    fflush(fp);
    fclose(fp);
}

static void load_super_block(void) {
    fp = fopen("vf.txt", "r+b");
    fseek(fp, DISK_START, SEEK_SET);
    fread(superblock, SB_SIZE, 1, fp);
    fclose(fp);
}

static unsigned short getRootEntry(void) {
    load_cur_block();
    if (cstate->free_user_num == 0) {
        cout<<"There is no User-Entry to be allocated!"<<endl;
        return 0;
    }
    for (int i = 1 ; i <= MAX_USER_NUM; i++) {
        load_root_entry_block(i);
        if (root_dir_item->inodeNum == 0) {
            cstate->free_user_num --;
            update_cur_block();
//            cout<<"root"<<i<<endl;
            return i;
        }
    }
    return 0;
}

static unsigned short getBlock(void) {
    load_cur_block();
    if (cstate->free_block_num == 0) {
        cout<<"There is no block to be allocated!"<<endl;
        return 0;
    }
    for (short k = 1 ; k <= DATA_BLOCK_NUM; k+= 8) {
        //文件最大分8块
        load_block(k);
        if (Buffer[0] == '\0') {
            cstate->free_block_num --;
            update_cur_block();
//            cout<<"block"<<k<<endl;
            return k;
        }
    }
//    cout<<"block 0"<<endl;
    return 0;
}

static unsigned short getNode(void) {
    load_cur_block();
    if (cstate->free_inode_num == 0) {
        cout<<"There is no Inode to be allocated!"<<endl;
        return 0;
    }
    for (int i = 1 ; i <=  INODE_BLOCK_NUM * INODE_NUM_PER_BLOCK ; i++) {
        load_inode_block(i);
        if (inode->i_block == 0) {
            //可用块
            cstate->free_inode_num --;
            update_cur_block();
//            cout<<"node"<<i<<endl;
            return i;
        }
    }
    return 0;
}

static unsigned short getEntry(void) {
    load_cur_block();
    if (cstate->free_entry_num == 0) {
        cout<<"There is no DIR-ENTRY to be allocated!"<<endl;
        return 0;
    }
    //每次最大4块最为目录项
    for (int i = 1 ; i <= ENTRY_BLOCK_NUM ; i+= 4) {
        load_entry_block(i);
//        cout<<dir_item[0].inodeNum<<endl;
        if (dir_item[0].inodeNum == 0) {
            //可用entry
            cstate->free_entry_num -= MAX_ENTRY_BLOCK;
            update_cur_block();
//            cout<<"entry"<<i<<endl;
            return i;
        }
    }
    return 0;
}

static void intialize_disk(void) {
    cout<<"Create EXT2 File System"<<"\n";
    cout<<"Please Wait..."<<endl;
    for (short i = 0; i < BLOCK_SIZE; i ++) {
        Buffer[i] = 0; //清空缓冲区
    }
    if (fp != NULL) {
        fclose(fp);
    }
    fp = fopen("vf.txt", "w+b");
    fseek(fp, DISK_START, SEEK_SET); //找到初始的位置
    for (short i = 0 ; i < DISK_SIZE ; i++) {
        fwrite(Buffer, BLOCK_SIZE, 1, fp);
    }
    load_super_block();
    superblock->volume_name = VOLUME_NAME;
    superblock->disk_size = DISK_SIZE;
    superblock->block_Size = BLOCK_SIZE;
    superblock->data_Block_Num = DATA_BLOCK_NUM;
    update_super_block();
    
    load_cur_block();
    cstate->free_block_num = DATA_BLOCK_NUM;
    cstate->free_entry_num = ENTRY_BLOCK_NUM;
    cstate->free_inode_num = INODE_BLOCK_NUM;
    cstate->free_user_num = MAX_USER_NUM;
    update_cur_block();
    
    load_mfd_block(1);
    mfd->user_name = "admin";
    mfd->pass_word = "admin";
    mfd->root_entry_num = getRootEntry();// 1号根目录项
    update_mfd_block(1);
    
    load_root_entry_block(mfd->root_entry_num);
    root_dir_item->file_name = "/";
    root_dir_item->name_length = sizeof(root_dir_item->file_name);
    root_dir_item->type = 2; //为目录
    root_dir_item->inodeNum = getNode();// 获取一个索引节点
    update_root_entry_block(mfd->root_entry_num);
    
    load_inode_block(root_dir_item->inodeNum); //修改索引节点
    inode->file_name = "/";
    inode->i_mode = 6; // 所有权限
    inode->i_size = 0;//初始文件大小为0
    inode->i_block_num = 1; //分配一个目录项
    inode->i_block = getEntry();
    inode->c_time = 0;
    inode->m_time = 0;
    inode->d_time = 0;
    update_inode_block(root_dir_item->inodeNum);
    
    load_entry_block(inode->i_block); // 加载根目录索引节点指向的磁盘块
    dir_item[0].file_name = ".";
    dir_item[0].name_length = sizeof(dir_item[0].file_name);
    dir_item[0].type = dir_item[1].type = 2; // 都为目录
    dir_item[0].inodeNum = dir_item[1].inodeNum = root_dir_item->inodeNum; // 根目录节点索引指向原来索引点
    dir_item[1].file_name = "..";
    dir_item[1].name_length = sizeof(dir_item[1].file_name);
    update_entry_block(inode->i_block);
    
    cstate->current_User = mfd->user_name;
    cstate->current_Path = root_dir_item->file_name;
    cstate->current_Inode = root_dir_item->inodeNum;
    update_cur_block();
    cout<<"EXT2FS File System estabulished!!"<<endl;
    check_disk();
    fclose(fp);
}

void check_disk(void) {
    load_super_block();
    cout<<"check disk:"<<endl;
    cout.setf(ios::left);
    cout.width(20);
    cout<<"  volume_name";
    cout.width(15);
    cout<<":"<<superblock->volume_name<<endl;
    cout.setf(ios::left);
    cout.width(20);
    cout<<"  disk_size";
    cout.width(15);
    cout<<":"<<superblock->disk_size<<"kb"<<endl;
    cout.setf(ios::left);
    cout.width(20);
    cout<<"  data_Block_Num";
    cout.width(15);
    cout<<":"<<superblock->data_Block_Num<<endl;
    cout.setf(ios::left);
    cout.width(20);
    cout<<"  block_Size";
    cout.width(15);
    cout<<":"<<superblock->block_Size<<"kb"<<endl;
}

void intialize_memory(void) {
    fp = fopen("vf.txt", "r+b");
    if (fp == NULL) {
        cout<<"File System Not Exists!!"<<endl;
        intialize_disk();
        fp = fopen("vf.txt", "r+b");
        return;
    }
}

bool login(string name, string pass) {
    for (int i = 1 ; i <= MAX_USER_NUM; i++) {
        load_mfd_block(i);
        load_cur_block();
        if (mfd->user_name == name && mfd->pass_word == pass) {
            load_root_entry_block(mfd->root_entry_num);
            cstate->current_User = name;
            cstate->current_Path = root_dir_item->file_name;
            cstate->current_Inode = root_dir_item->inodeNum;
            update_cur_block();
            return true;
        }
    }
    return false;
}

void dir(void) {
    cout.setf(ios::left);
    cout.width(6);
    cout<<"mode";
    cout.width(8);
    cout<<"type";
    cout.width(20);
    cout<<"item";
    cout.width(25);
    cout<<"time";
    cout.width(4);
    cout<<"size"<<endl;
    load_cur_block();
    unsigned short current_inode = cstate->current_Inode;
    load_inode_block(current_inode);
    for (short i = 0; i < inode->i_block_num; i++) {
        load_entry_block(inode->i_block + i);
        for (auto item : dir_item) {
            if (item.inodeNum != 0) {
                load_inode_block(item.inodeNum);
                cout.setf(ios::left);
                cout.width(6);
                if (inode->i_mode == 7) {
                    cout<<"rwx";
                } else if (inode->i_mode == 6) {
                    cout<<"rw-";
                } else if (inode->i_mode == 5) {
                     cout<<"r-x";
                } else if (inode->i_mode == 4) {
                    cout<<"r--";
                } else if (inode->i_mode == 3) {
                    cout<<"-wx";
                } else if (inode->i_mode == 2) {
                    cout<<"-w-";
                } else if (inode->i_mode == 1) {
                    cout<<"--x";
                } else if (inode->i_mode == 0) {
                    cout<<"---";
                }
                cout.width(8);
                if (item.type == 1) {
                    cout<<"<FILE>";
                } else {
                    cout<<"<DIR>";
                }
                cout.width(20);
                cout<<item.file_name;
                cout.width(25);
                if (item.file_name != "." && item.file_name != "..") {
                    auto tm = *localtime(&inode->m_time);
                    char s[100];
                    strftime(s, sizeof(s), "%Y/%m/%d %H:%M:%S", &tm);
                    cout<<s;
                } else {
                    cout<<"----";
                }
                if (item.file_name != "." && item.file_name != "..") {
                    cout<<inode->i_size<<"kb"<<endl;
                } else {
                    cout<<"----"<<endl;
                }
            }
        }
    }
}

//完成
void cd(string tmp) {
    load_cur_block();
    unsigned short current_inode = cstate->current_Inode;
    load_inode_block(current_inode);
    for (short i = 0; i < inode->i_block_num; i++) {
        load_entry_block(inode->i_block + i);
        for (auto item : dir_item) {
            if (item.type != 1 && item.file_name == tmp && item.inodeNum != 0) {
                load_inode_block(item.inodeNum);
                cstate->current_Path = inode->file_name;
                cstate->current_Inode = item.inodeNum;
                update_cur_block();
                return;
            }
        }
    }
    cout<<"cd: "<<tmp<<" : Not a directory"<<endl;
    return;
}

void md(string tmp) {
    load_cur_block();
    unsigned short current_inode = cstate->current_Inode;
    load_inode_block(current_inode);//得到当前的索引号
    for (short i = 0; i < MAX_ENTRY_BLOCK; i++) {
        load_entry_block(inode->i_block + i);
        for (int j = 0 ; j < sizeof(dir_item)/sizeof(dir_item[0]) ; j++) {
            if (dir_item[j].file_name == tmp && dir_item[j].inodeNum != 0 && dir_item[j].type == 2) {
                cout<<"md: "<<tmp<<" : Dir exist!"<<endl;
                return;
            }
            if (dir_item[j].inodeNum == 0) {
                //找到一块未分配的目录项,修改目录项
                unsigned short nodetmp = getNode();
                load_inode_block(current_inode);
                load_entry_block(inode->i_block + i);
                dir_item[j].file_name = tmp;
                dir_item[j].type = 2; //目录项
                dir_item[j].name_length = sizeof(dir_item[j].file_name);
                dir_item[j].inodeNum = nodetmp;
                update_entry_block(inode->i_block + i);
                
                unsigned short entrytmp =  getEntry();
                load_inode_block(current_inode);
                load_entry_block(inode->i_block + i);
                load_inode_block(dir_item[j].inodeNum);
                inode->file_name = tmp;
                inode->i_mode = 6;
                inode->i_size = ENTRY_NUM_PER_BLOCK;
                inode->i_block_num = 1;//初始目录项都为一块
                time_t time = std::time(NULL);
                inode->m_time = time;
                inode->c_time = time;
                inode->d_time = time;
                inode->i_block = entrytmp;
                update_inode_block(dir_item[j].inodeNum);
                
                load_entry_block(inode->i_block);
                dir_item[0].file_name = ".";
                dir_item[0].name_length = sizeof(dir_item[0].file_name);
                dir_item[0].type = dir_item[1].type = 2; // 都为目录
                dir_item[0].inodeNum = nodetmp; // 根目录节点索引指向原来索引点
                
                dir_item[1].file_name = "..";
                dir_item[1].name_length = sizeof(dir_item[1].file_name);
                dir_item[1].inodeNum = current_inode;
                update_entry_block(inode->i_block);
                
                load_inode_block(current_inode);
                if (i >= inode->i_block_num) {
                    inode->i_block_num++;
                    inode->i_size += ENTRY_NUM_PER_BLOCK;
                }
                update_inode_block(current_inode);
                return;
            }
        }
    }
}

void rd(string tmp) {
    load_cur_block();
    unsigned short current_inode = cstate->current_Inode;
    load_inode_block(current_inode);
    for (short i = 0; i < inode->i_block_num; i++) {
        load_entry_block(inode->i_block + i);
        for (short j = 0 ; j < sizeof(dir_item)/sizeof(dir_item[0]); j++) {
            if (dir_item[j].file_name == tmp && dir_item[j].type != 1 && dir_item[j].inodeNum != 0) {
                load_inode_block(dir_item[j].inodeNum);
                for (short k = 0; k < inode->i_block_num; k++) {
                    load_entry_block(inode->i_block + k);
                    for (auto item : dir_item ) {
                        if (item.file_name != "." && item.file_name != ".." && item.file_name != "" && item.inodeNum != 0) {
                            cout<<"rd: "<<tmp<<" : Not a empty Directory!"<<endl;
                            return;
                        }
                    }
                }
                load_inode_block(current_inode);
                load_entry_block(inode->i_block + i);
                load_inode_block(dir_item[j].inodeNum);
                fp = fopen("vf.txt", "r+b");
                fseek(fp, DIR_START * BLOCK_SIZE + (inode->i_block - 1)* BLOCK_SIZE, SEEK_SET);
                for (short k = 0; k < BLOCK_SIZE; k ++) {
                    Buffer[k] = 0; //清空缓冲区
                }
                fwrite(Buffer, BLOCK_SIZE, 1, fp);//清空目录项
                fflush(fp);
                fclose(fp);
                //所有inode的i_block = 0的可以覆盖利用
                inode->i_block = 0;
                update_inode_block(dir_item[j].inodeNum);
                //所有entry的inodeNum = 0 则可以覆盖利用，dir[0].inodeNum = 0
                dir_item[j].inodeNum = 0;
                load_inode_block(current_inode);
                update_entry_block(inode->i_block + i);
                return;
            }
        }
    }
    cout<<"rd: "<<tmp<<" : No such Directory!"<<endl;
    return;
}

//创建文件项
void  create(string tmp) {
    load_cur_block();
    unsigned short current_inode = cstate->current_Inode;
    load_inode_block(current_inode);
    for (short i = 0; i < MAX_ENTRY_BLOCK; i++) {
        load_entry_block(inode->i_block + i);
        for (int j = 0 ; j < sizeof(dir_item)/sizeof(dir_item[0]) ; j++) {
            if (dir_item[j].file_name == tmp && dir_item[j].type == 1 && dir_item[j].inodeNum != 0) {
                cout<<"create: "<<tmp<<" : File exist!"<<endl;
                return;
            }
            if (dir_item[j].inodeNum == 0) {
                unsigned short nodetmp = getNode();
                load_inode_block(current_inode);
                load_entry_block(inode->i_block + i);
                dir_item[j].file_name = tmp;
                dir_item[j].type = 1; //普通文件
                dir_item[j].name_length = sizeof(dir_item[j].file_name);
                dir_item[j].inodeNum = nodetmp;
                update_entry_block(inode->i_block + i);
    
                load_inode_block(dir_item[j].inodeNum);
                inode->file_name = tmp;
                inode->i_mode = 7;//权限
                inode->i_size = 0;//文件初大小为0
                inode->i_block_num = 1;//初始占用大小为1块,最大为8块
                time_t time = std::time(NULL);
                inode->m_time = time;
                inode->c_time = time;
                inode->d_time = time;
                inode->i_block = getBlock();
                update_inode_block(dir_item[j].inodeNum);
                
                load_block(inode->i_block);
                memset(Buffer, 0, BLOCK_SIZE);
                strcpy(Buffer, "Empty File!");
                update_block(inode->i_block);

                load_inode_block(current_inode);
                if (i >= inode->i_block_num) {
                    inode->i_block_num++;
                }
                update_inode_block(current_inode);
                
                return;
            }
        }
    }
}

//删除文件
void del(string tmp) {
    load_cur_block();
    unsigned short current_inode = cstate->current_Inode;
    load_inode_block(current_inode);
    for (short i = 0; i < inode->i_block_num; i++) {
        load_entry_block(inode->i_block + i);
        for (short j = 0 ; j < sizeof(dir_item)/sizeof(dir_item[0]); j++) {
            if (dir_item[j].file_name == tmp && dir_item[j].type != 2 && dir_item[j].inodeNum != 0) {
                //普通文件
                load_inode_block(dir_item[j].inodeNum);
                for (short k = 0; k < inode->i_block_num; k++) {
                    load_block(inode->i_block + k);//加载对应的数据块
                    for (short p = 0; p < BLOCK_SIZE; p++) {
                        Buffer[p] = 0;
                    }
                    update_block(inode->i_block + k);//清空对应的数据块
                }
                inode->i_block = 0;
                time_t time = std::time(NULL);
                inode->d_time = time;
                update_inode_block(dir_item[j].inodeNum);
                load_inode_block(current_inode);
                load_entry_block(inode->i_block + i);
                dir_item[j].inodeNum = 0;
                update_entry_block(inode->i_block + i);
                return;
            }
        }
    }
    cout<<"del: "<<tmp<<" : No such File!"<<endl;
    return;
}

void open(string tmp) {
    load_cur_block();
    unsigned short current_inode = cstate->current_Inode;
    load_inode_block(current_inode);
    for (short i = 0; i < inode->i_block_num; i++) {
        load_entry_block(inode->i_block + i);
        for (auto item : dir_item) {
            if(item.file_name == tmp && item.inodeNum != 0 && item.type == 1) {
                if (fopenFile.size() > 16) {
                    cout<<"FileOpenTable is full, Please close one file !"<<endl;
                }
                fopenFile.insert(pair<string, unsigned short> (tmp, item.inodeNum));
                cout<<"File: "<<tmp<<" has opened!"<<endl;
                return;
            }
        }
    }
    cout<<"open: "<<tmp<<" : No such File!"<<endl;
    return;
}

void close(string tmp) {
    map<string, unsigned short>::iterator map_iter;
    map_iter = fopenFile.find(tmp);
    if (map_iter == fopenFile.end())
        cout<<"close: "<<tmp<<" : No such File Open!"<<endl;
    else
        fopenFile.erase(map_iter);
    return;
}

void read(string tmp) {
    map<string, unsigned short>::iterator map_iter;
    map_iter = fopenFile.find(tmp);
    if (map_iter == fopenFile.end()) {
        cout<<"read: "<<tmp<<" : No such File Open!"<<endl;
        return;
    } else {
        unsigned short inodetmp = map_iter->second;
        load_inode_block(inodetmp);
        for (int i = 0 ; i < inode->i_block_num; i++) {
            load_block(inode->i_block + i);
            cout<<Buffer;
        }
        cout<<endl;
    }
}

void write(string tmp) {
    map<string, unsigned short>::iterator map_iter;
    map_iter = fopenFile.find(tmp);
    if (map_iter == fopenFile.end()) {
        cout<<"write: "<<tmp<<" : No such File Open!"<<endl;
        return;
    } else {
        unsigned short inodetmp = map_iter->second;
        memset(tempFileBuf, 0, BLOCK_SIZE * MAX_DATA_BLOCK);
        char c;
        int local = 0;
        while ((c = getchar()) != '#') {
            if (local >= BLOCK_SIZE * MAX_DATA_BLOCK) {
                cout<<"Reach Max File Size!!"<<endl;
                break;
            }
            tempFileBuf[local++] = c;
        }
        char tmp[8 * 512];
        for (int i = 0; i < sizeof(tempFileBuf)/sizeof(tempFileBuf[0]); i++) {
            tmp[i] = tempFileBuf[i + 1];
        }
        unsigned short num = local / BLOCK_SIZE + 1;
        load_inode_block(inodetmp);
        inode->i_size = local;//字节大小
        time_t time = std::time(NULL);
        inode->m_time = time;
        update_inode_block(inodetmp);
        
        for (int k = 0 ; k < num; k++) {
            load_block(inode->i_block + k);
            for (int j = 0; j < BLOCK_SIZE; j++) {
                Buffer[j] = tmp[j + k * BLOCK_SIZE];
            }
            update_block(inode->i_block + k);
        }
    }
}

void useradd(string tmp) {
    string password;
    for (int i = 1 ; i <= MAX_USER_NUM; i++) {
        load_mfd_block(i);
        if (mfd->root_entry_num == 0) {
            mfd->user_name = tmp;
            cout<<"Change password for user: "<<tmp<<endl;
            cin>>password;
            mfd->pass_word = password;
            mfd->root_entry_num = getRootEntry();// 1号根目录项
            update_mfd_block(i);
            
            load_root_entry_block(mfd->root_entry_num);
            root_dir_item->file_name = "/";
            root_dir_item->name_length = sizeof(root_dir_item->file_name);
            root_dir_item->type = 2; //为目录
            root_dir_item->inodeNum = getNode();// 获取一个索引节点
            update_root_entry_block(mfd->root_entry_num);
            
            load_inode_block(root_dir_item->inodeNum); //修改索引节点
            inode->file_name = "/";
            inode->i_mode = 6; // 所有权限
            inode->i_size = 0;//初始文件大小为0
            inode->i_block_num = 1; //分配一个目录项
            inode->i_block = getEntry();
            inode->c_time = 0;
            inode->m_time = 0;
            inode->d_time = 0;
            update_inode_block(root_dir_item->inodeNum);
            
            load_entry_block(inode->i_block); // 加载根目录索引节点指向的磁盘块
            dir_item[0].file_name = ".";
            dir_item[0].name_length = sizeof(dir_item[0].file_name);
            dir_item[0].type = dir_item[1].type = 2; // 都为目录
//            cout<<root_dir_item->inodeNum;
            dir_item[0].inodeNum = dir_item[1].inodeNum = root_dir_item->inodeNum; // 根目录节点索引指向原来索引点
            dir_item[1].file_name = "..";
            dir_item[1].name_length = sizeof(dir_item[1].file_name);
            update_entry_block(inode->i_block);
            
            cout<<"useradd: "<<tmp<<" : Has been created!"<<endl;
            return;
        }
    }
    cout<<"useradd: "<<tmp<<" : Reach Max num! "<<endl;
    return;
}

void su(string tmp) {
    string password;
    cout<<"password: ";
    cin>>password;
    login(tmp, password);
}

void help(void) {
    cout<<" Available command here~"<<endl;
    cout.setf(ios::left);
    cout.width(20);
    cout<<"  Create dir: ";
    cout.width(20);
    cout<<" md -dirname";
    cout.setf(ios::left);
    cout.width(20);
    cout<<"  Delete dir: ";
    cout.width(20);
    cout<<" rd -dirname"<<endl;
    cout.setf(ios::left);
    cout.width(20);
    cout<<"  Create file: ";
    cout.width(20);
    cout<<" create -filename";
    cout.setf(ios::left);
    cout.width(20);
    cout<<"  Delete file: ";
    cout.width(20);
    cout<<" del -filename"<<endl;
    cout.setf(ios::left);
    cout.width(20);
    cout<<"  List items: ";
    cout.width(20);
    cout<<" dir";
    cout.setf(ios::left);
    cout.width(20);
    cout<<"  Change dir: ";
    cout.width(20);
    cout<<" cd -dirname"<<endl;
    cout.setf(ios::left);
    cout.width(20);
    cout<<"  Open file: ";
    cout.width(20);
    cout<<" open -filename";
    cout.setf(ios::left);
    cout.width(20);
    cout<<"  Close file: ";
    cout.width(20);
    cout<<" close -filename"<<endl;
    cout.setf(ios::left);
    cout.width(20);
    cout<<"  Read file: ";
    cout.width(20);
    cout<<" read -filename";
    cout.setf(ios::left);
    cout.width(20);
    cout<<"  Write file: ";
    cout.width(20);
    cout<<" write -filename"<<endl;
    cout.setf(ios::left);
    cout.width(20);
    cout<<"  Add user: ";
    cout.width(20);
    cout<<" useradd -username";
    cout.setf(ios::left);
    cout.width(20);
    cout<<"  Change user: ";
    cout.width(20);
    cout<<" su -username"<<endl;
    cout.setf(ios::left);
    cout.width(20);
    cout<<"  Formate disk: ";
    cout.width(20);
    cout<<" format";
    cout.setf(ios::left);
    cout.width(20);
    cout<<"  Check dist: ";
    cout.width(20);
    cout<<" ckdisk"<<endl;
}

void ckdisk(void) {
    check_disk();
}

void format(void) {
    cout<<endl<<"Format ext2 file system......."<<endl;
    intialize_disk();
    intialize_memory();
}
#endif /* FuncState_h */

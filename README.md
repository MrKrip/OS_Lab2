
# OS Lab 2 - Page Allocator


## Main part


��� ����������� ������ ������� �� �������. ������ �������� ����� ���������� � ����������� ������ ��� �� ������� ����� (��������������� ��������, � ������� ���� ���� �� ���� ���������, �.�. �������� ��� ������� ���� �������� ������� ����������� ������). ��� ��������� � ��������, ������� ��������� �� ������� �����, ���������� ���������� ������ � ������������ ������� ������� ��������� ���������� �������� � ��������� � �� ���������� �� �������� �����. ������ �������� ������ ����� �� 4 ����� �� ���������� �����. ����� �������, ���� � �������� �������� ������� ��������� ������ ���������� � �������� ����� �������, ��� ������ �� ��������� ���� � ������, ��� �� �����������.
### ���������
**PageSize**-����������� ������ ��������(4096)
**MinBlockSiz�**-����������� ������ �����(16)

####��������� �������

**Free**-�������� �� ������ ������ ��������

**Divided**-�������� ���� ��������� �� ����� ������ ������.����� ������ ������ ����� ���������� ������.��������, ���� ������ ����� ���� ����� 2 x (x ? 4).

**Multipage**-����� �������� � ���� ��� ��������� �������. ���� ���������� ����������� ���� ������ ���������� ������� �������� ��� �������� ��������, �� ��������� ��������� ���� ������ �� ���������� ������ ����� �������.


#### ������� ���������

```
#void * mem_alloc (size_t size)#
```

������� �������� ���� ������ ��������� ������� � size ����. ���� ���� ������ ��� ������� �������, �� ���������� ����� ������ ����� �����, � ��������� ������ ���������� NULL.

### ������� �����������

```
void * mem_realloc (void * addr, size_t size)
```

������� �������� ������ ����� ������ � ������� addr �� size ����. ��� ���� ���������� (�� ��� �����) ������� ����� ������ ����������� � ������ ���� ������.


### Free memory function
```
void mem_free (void * addr)
```

������� ����������� ������ ���������� ���� ������.

## ������������

### �������� ����������
#### �� ������� ��������� � 10 ����������

##### ���
```
PageAllocator aloc = PageAllocator(10*PageSize);
aloc.mem_dump();
```
##### ���������
(pictures/PageAlloc.PNG)

�� ������ ������ ��� 10 ��������� ��������

### ��������� ������ ������ �������� �������
#### �� ������� 3 ����� �� 200 ����, 1 ���� 300 ���� � 1 ������� ���� �� 3 ��������

##### ���
```
void* a1 =aloc.mem_alloc(200);
	void* a2 = aloc.mem_alloc(300);
	void* a3 = aloc.mem_alloc(200);
	void* a4 = aloc.mem_alloc(200);
	void* a5 = aloc.mem_alloc(3*PageSize);
	aloc.mem_dump();
```

##### ���������
(pictures/mem_alloc.PNG)

������ ������������� ������

### ������������ ������ ���������� ������
#### �� ����������� 1 ���� �� 200 ���� � 1 ���� �� 3 ��������

##### ���
```
aloc.mem_free(a1);
	aloc.mem_free(a5);
	aloc.mem_dump();
```
������ ������������ ������
##### ���������
(pictures/mem_free.PNG)
---
### ��������� ������� �����
#### �� �������� ������ ����� � 300 ���� �� ������� 4 �������
##### ���
```
void* a6 = aloc.mem_realloc(a2,4*PageSize);
	aloc.mem_dump();
```

##### ���������
(pictures/mem_realoc.PNG)
---


#include <check.h>
#include <stdio.h>
#include <stdlib.h>
START_TEST(test1){
	int m = 6;
	int number = 3;
	int test_number = 0;
	FILE *file = fopen("lab3.txt", "r");
	int test_equals;
	test_equals = 0;
	int numbers[number/3];
	for(int i = 0; i < number/3; i++){
		numbers[i] = i + number/3 + 1;
	}
	char line[10];
	while(fgets(line,m*number*10, file) != NULL){
		int a = atoi(line);
		test_number++;
		for(int i = 0; i < a; i++){
			if (a == numbers[i]){
				test_equals++;
			}
		}
	}
	fclose(file);

	ck_assert(test_number == m*number);
	for(int i = 0; i < number; i++){
		ck_assert(numbers[i] == (3*m));
	}
}
END_TEST

int main(void){
	Suite *s = suite_create("suit");
	TCase *tc = tcase_create("test case");
	SRunner *sr = srunner_create(s);
	tcase_add_test(tc, test1);
	srunner_run_all(sr, CK_NORMAL);
	int failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return failed == 0 ? 0 : 1;
}

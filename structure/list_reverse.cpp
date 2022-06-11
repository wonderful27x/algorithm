lint *reverse(list *head) {
	if(head == nullptr || head->next == nullptr) return head;
	list *beg = nullptr;
	list *mid = head;
	list *end = head->next;
	while(true) {
		mid->next = beg;
		if(end == nullptr)break;
		beg = mid;
		mid = end;
		end = end->next;
	}
	head = mid;
	return head;
}

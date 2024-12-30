
# Global variables names
COMMUNICATION_CONNECTION_TESTS_EXECUTABLE_NAME = communication_connection_tests
PRIORIRTY_QUEUE_TESTS_EXECUTABLE_NAME = priority_queue_tests

priority_queue_tests:

	# Compile with the priority queue tests flag
	cmake -S ../../ -B ../../build/ -DNODE=ON -DPRIORIRTY_QUEUE_TESTS=ON -DASAN=ON --fresh
	make -C ../../build/
	
	kitty --detach fish -c "cd ../../build/; ./$(PRIORIRTY_QUEUE_TESTS_EXECUTABLE_NAME); exec fish"

communication_connection_tests:

	# Compile with the priority queue tests flag
	cmake -S ../../ -B ../../build/ -DNODE=ON -DCOMMUNICATION_CONNECTION_TESTS=ON -DASAN=ON --fresh
	make -C ../../build/
	
	kitty --detach fish -c "cd ../../build/; ./$(COMMUNICATION_CONNECTION_TESTS_EXECUTABLE_NAME); exec fish"

all: priority_queue_tests communication_connection_tests
	
	







	# # Compile the node 8080
	# cmake -S ../../ -B ../../build/ -DASAN=ON -DNODE=ON -DNODE_8080=ON --fresh
	#
	# make -C ../../build/
	#
	# mv ../../build/Spyke_Blockchain ../../build/node_8080
	#
	# # Compile the node 8081
	# cmake -S ../../ -B ../../build/ -DASAN=ON -DNODE=ON -DNODE_8081=ON --fresh
	#
	# make -C ../../build/
	#
	# mv ../../build/Spyke_Blockchain ../../build/node_8081
	#
	# # Start both nodes in diferent shells
	# kitty --detach fish -c "cd ../../build/; ./node_8080; exec fish"
	# sleep .5
	# kitty --detach fish -c "cd ../../build/; ./node_8081; exec fish"

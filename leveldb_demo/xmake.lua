set_project("leveldb_demo")
set_languages("c++17")

add_rules("mode.debug", "mode.release")

add_requires("leveldb")

set_kind("binary")
add_packages("leveldb")

function all_examples()
    local res = {}
    for _, x in ipairs(os.files("**.cc")) do
        local item = {}
        local s = path.filename(x)
        table.insert(item, s:sub(1, #s - 3))       -- target
        table.insert(item, path.relative(x, "."))  -- source
        table.insert(res, item)
    end
    return res
end

for _, example in ipairs(all_examples()) do
target(example[1])
    add_files(example[2])
end


